defmodule HakoAssetImpl do
  use Agent

  alias HakoAssetImpl
  alias HakoApi
  alias HakoSimevent
  import Jason

  # HakoAssetインスタンスの初期化
  def start_link(_) do
    initial_state = %{
      external_use: false,
      is_initialized: false,
      asset_instance: %{
        asset_name: nil,
        config_path: nil,
        delta_usec: 0,
        current_usec: 0,
        robots: [],
        callback: nil,
        param: %{}
      }
    }
    Agent.start_link(fn -> initial_state end, name: __MODULE__)
  end

  # external_useフラグの設定と取得
  def set_external_use(state) do
    Agent.update(__MODULE__, fn instance -> %{instance | external_use: state} end)
  end

  def get_external_use() do
    Agent.get(__MODULE__, & &1.external_use)
  end

  # asset_instanceの設定と取得
  def set_asset_instance(new_asset_instance) do
    Agent.update(__MODULE__, fn instance -> %{instance | asset_instance: new_asset_instance} end)
  end

  def get_asset_instance() do
    Agent.get(__MODULE__, & &1.asset_instance)
  end

  # asset_instanceの初期化
  def reset_asset_instance() do
    Agent.update(__MODULE__, fn instance ->
      %{instance |
        asset_instance: %{
          asset_name: nil,
          config_path: nil,
          delta_usec: 0,
          current_usec: 0,
          callback: nil,
          robots: [],
          param: %{}
        },
        is_initialized: false
      }
    end)
  end

  # ロボットのPDUリーダーを作成
  defp create_reader(reader_json) do
    %{
      type: reader_json["type"],
      org_name: reader_json["org_name"],
      name: reader_json["name"],
      channel_id: reader_json["channel_id"],
      pdu_size: reader_json["pdu_size"],
      write_cycle: reader_json["write_cycle"]
    }
  end

  # ロボットのPDUライターを作成
  defp create_writer(writer_json) do
    %{
      type: writer_json["type"],
      org_name: writer_json["org_name"],
      name: writer_json["name"],
      write_cycle: writer_json["write_cycle"],
      channel_id: writer_json["channel_id"],
      pdu_size: writer_json["pdu_size"],
      method_type: writer_json["method_type"]
    }
  end

  # ロボットの設定を解析してPDUリーダー・ライターをセットアップ
  defp parse_robots(is_plant, robots_json) do
    Enum.map(robots_json, fn robot_json ->
      %{
        name: robot_json["name"],
        pdu_writers: parse_pdu_writers(is_plant, robot_json),
        pdu_readers: parse_pdu_readers(is_plant, robot_json)
      }
    end)
  end

  defp parse_pdu_writers(is_plant, robot_json) do
    pdu_writers = []

    if Map.has_key?(robot_json, "shm_pdu_writers") do
      pdu_writers = pdu_writers ++ parse_writer_or_reader(is_plant, robot_json["shm_pdu_writers"])
    end

    if Map.has_key?(robot_json, "rpc_pdu_writers") do
      pdu_writers = pdu_writers ++ parse_writer_or_reader(is_plant, robot_json["rpc_pdu_writers"])
    end

    pdu_writers
  end

  defp parse_pdu_readers(is_plant, robot_json) do
    pdu_readers = []

    if Map.has_key?(robot_json, "shm_pdu_readers") do
      pdu_readers = pdu_readers ++ parse_writer_or_reader(is_plant, robot_json["shm_pdu_readers"], false)
    end

    if Map.has_key?(robot_json, "rpc_pdu_readers") do
      pdu_readers = pdu_readers ++ parse_writer_or_reader(is_plant, robot_json["rpc_pdu_readers"], false)
    end

    pdu_readers
  end

  defp parse_writer_or_reader(is_plant, json_array, is_writer \\ true) do
    Enum.map(json_array, fn entry_json ->
      if is_plant do
        if is_writer do
          create_writer(entry_json)
        else
          create_reader(entry_json)
        end
      else
        if is_writer do
          create_reader(entry_json)
        else
          create_writer(entry_json)
        end
      end
    end)
  end
  defp create_pdu_channels(robots) do
    Enum.each(robots, fn robot ->
      Enum.each(robot.pdu_writers, fn writer ->
        IO.puts("Robot: #{robot.name}, PduWriter: #{writer.name}")
        IO.puts("channel_id: #{writer.channel_id}, pdu_size: #{writer.pdu_size}")

        case HakoApi.create_pdu_lchannel(robot.name, writer.channel_id, writer.pdu_size) do
          true ->
            :ok
          false ->
            IO.puts("ERROR: Can not create_pdu_channel()")
            {:error, "Failed to create PDU channel"}
        end
      end)
    end)
  end

  # Assetの初期化
  def init(asset_name, config_path, delta_usec, is_plant) do
    reset_asset_instance()

    asset_instance = get_asset_instance()

    if File.exists?(config_path) do
      {:ok, file_content} = File.read(config_path)
      parsed_json = Jason.decode!(file_content)

      robots = parse_robots(is_plant, parsed_json["robots"])

      updated_instance = %{
        asset_instance |
        asset_name: asset_name,
        config_path: config_path,
        delta_usec: delta_usec,
        current_usec: 0,
        param: parsed_json,
        robots: robots,
      }

      if HakoApi.init_asset() == false or HakoApi.simevent_init() == false do
        {:error, "Hako master or Simevent controller not found"}
      else
        set_asset_instance(updated_instance)

        # Assetの登録処理
        case HakoApi.register_asset(asset_name) do
          true ->
            # PDUチャネルの作成
            create_pdu_channels(updated_instance.robots)
            :ok
          false ->
            {:error, "Failed to register asset"}
        end
      end
    else
      {:error, "Config file not found"}
    end
  end

  # Callbackの登録
  def register_callback(callbacks) do
    asset_instance = get_asset_instance()

    if get_external_use() do
      {:error, "External use is enabled; cannot register callbacks"}
    else
      updated_instance = %{asset_instance | callback: callbacks}
      set_asset_instance(updated_instance)
      :ok
    end
  end

  # 外部使用用の初期化
  def initialize_for_external() do
    asset_instance = get_asset_instance()

    if asset_instance.is_initialized do
      {:error, "Asset is already initialized"}
    else
      updated_instance = %{
        asset_instance |
        is_initialized: true,
        external_use: true,
        asset_name: "None",
        delta_usec: 0,
        current_usec: 0,
      }

      set_asset_instance(updated_instance)
      :ok
    end
  end
end
