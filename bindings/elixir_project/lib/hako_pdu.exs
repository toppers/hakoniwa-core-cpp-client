defmodule PduBinaryConverter do
  defstruct offmap: nil, pdudef: nil

  def new(offset_path, pdudef_path) do
    offmap = OffsetMap.create_offmap(offset_path)
    pdudef = load_json(pdudef_path)
    %PduBinaryConverter{offmap: offmap, pdudef: pdudef}
  end

  defp load_json(path) do
    case File.read(path) do
      {:ok, content} -> Jason.decode!(content)
      {:error, reason} -> IO.puts("ERROR: #{reason}"); nil
    end
  end

  defp find_robo(%PduBinaryConverter{pdudef: pdudef}, robo_name) do
    Enum.find(pdudef["robots"], fn entry -> entry["name"] == robo_name end)
  end

  defp find_channel_in_container(container, channel_id) do
    Enum.find(container, fn entry -> entry["channel_id"] == channel_id end)
  end

  defp find_channel(conv, robo, channel_id) do
    containers = ["rpc_pdu_readers", "rpc_pdu_writers", "shm_pdu_readers", "shm_pdu_writers"]

    Enum.reduce_while(containers, nil, fn container_name, _acc ->
      case Map.get(robo, container_name) do
        nil -> {:cont, nil}
        container ->
          case find_channel_in_container(container, channel_id) do
            nil -> {:cont, nil}
            entry -> {:halt, entry}
          end
      end
    end)
  end

  defp find_pdu(conv, robo_name, channel_id) do
    with robo <- find_robo(conv, robo_name),
         channel <- find_channel(conv, robo, channel_id) do
      channel
    else
      _ -> IO.puts("ERROR: can not find robo_name=#{robo_name} or channel_id=#{channel_id}"); nil
    end
  end

  def bin2json(conv, robo_name, channel_id, binary_data) do
    case find_pdu(conv, robo_name, channel_id) do
      nil -> nil
      pdu -> BinaryReader.binary_read(conv.offmap, pdu["type"], binary_data)
    end
  end

  def json2bin(conv, robo_name, channel_id, json_data) do
    case find_pdu(conv, robo_name, channel_id) do
      nil -> nil
      pdu ->
        binary_data = <<>>
        BinaryWriter.binary_write(conv.offmap, binary_data, json_data, pdu["type"])
        binary_data
    end
  end

  def pdu_size(conv, robo_name, channel_id) do
    case find_pdu(conv, robo_name, channel_id) do
      nil -> nil
      pdu -> pdu["pdu_size"]
    end
  end

  def create_pdu_bin_zero(conv, robo_name, channel_id) do
    case find_pdu(conv, robo_name, channel_id) do
      nil -> nil
      pdu -> <<0::size(pdu["pdu_size"])-unit(8)>>
    end
  end

  def create_pdu_json_zero(conv, robo_name, channel_id) do
    case find_pdu(conv, robo_name, channel_id) do
      nil -> nil
      pdu ->
        binary_data = <<0::size(pdu["pdu_size"])-unit(8)>>
        BinaryReader.binary_read(conv.offmap, pdu["type"], binary_data)
    end
  end
end

defmodule HakoPdu do
  alias HakoAsset

  defstruct conv: nil, robot_name: nil, channel_id: nil, pdu_size: nil, obj: nil

  def new(conv, robot_name, channel_id) do
    pdu_size = PduBinaryConverter.pdu_size(conv, robot_name, channel_id)
    obj = PduBinaryConverter.create_pdu_json_zero(conv, robot_name, channel_id)
    %HakoPdu{conv: conv, robot_name: robot_name, channel_id: channel_id, pdu_size: pdu_size, obj: obj}
  end

  def get(pdu), do: pdu.obj

  def write(pdu) do
    data = PduBinaryConverter.json2bin(pdu.conv, pdu.robot_name, pdu.channel_id, pdu.obj)
    HakoAsset.pdu_write(pdu.robot_name, pdu.channel_id, data, byte_size(data))
  end

  def read(pdu) do
    data = HakoAsset.pdu_read(pdu.robot_name, pdu.channel_id, pdu.pdu_size)

    if data == nil do
      IO.puts("ERROR: hako_asset_pdu_read")
      nil
    else
      PduBinaryConverter.bin2json(pdu.conv, pdu.robot_name, pdu.channel_id, data)
    end
  end
end

defmodule HakoPduManager do
  defstruct conv: nil

  def new(offset_path, pdudef_path) do
    conv = PduBinaryConverter.new(offset_path, pdudef_path)
    %HakoPduManager{conv: conv}
  end

  def get_pdu(manager, robot_name, channel_id) do
    HakoPdu.new(manager.conv, robot_name, channel_id)
  end
end

defmodule Main do
  def main(args) do
    if length(args) != 7 do
      IO.puts("Usage: pdu_io.ex <offset_path> <pdudef.json> <robo_name> <channel_id> {r|w} <io_file>")
      System.halt(1)
    end

    [offset_path, pdudef_path, robo_name, channel_id, io_type, io_file] = args
    channel_id = String.to_integer(channel_id)
    manager = HakoPduManager.new(offset_path, pdudef_path)

    case io_type do
      "w" ->
        json_data = File.read!(io_file) |> Jason.decode!()
        binary_data = PduBinaryConverter.json2bin(manager.conv, robo_name, channel_id, json_data)

        if binary_data != nil do
          File.write!("./binary.bin", binary_data)
          IO.puts(Base.encode16(binary_data))
        else
          IO.puts("ERROR: Conversion failed or channel not found.")
        end

      "r" ->
        binary_data = File.read!(io_file)
        json_data = PduBinaryConverter.bin2json(manager.conv, robo_name, channel_id, binary_data)

        if json_data != nil do
          IO.puts(Jason.encode!(json_data, pretty: true))
        else
          IO.puts("ERROR: Conversion failed or channel not found.")
        end

      _ ->
        IO.puts("ERROR: Invalid IO type")
    end
  end
end

# 実行
args = System.argv()
Main.main(args)
