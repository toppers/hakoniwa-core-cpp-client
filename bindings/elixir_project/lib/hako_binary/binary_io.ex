defmodule PduConversion do
  def bin_to_int8(binary), do: :binary.decode_signed(binary, :little)
  def bin_to_int16(binary), do: :binary.decode_signed(binary, :little)
  def bin_to_int32(binary), do: :binary.decode_signed(binary, :little)
  def bin_to_bool(binary), do: :binary.decode_signed(binary, :little)
  def bin_to_int64(binary), do: :binary.decode_signed(binary, :little)
  def bin_to_uint8(binary), do: :binary.decode_unsigned(binary, :little)
  def bin_to_uint16(binary), do: :binary.decode_unsigned(binary, :little)
  def bin_to_uint32(binary), do: :binary.decode_unsigned(binary, :little)
  def bin_to_uint64(binary), do: :binary.decode_unsigned(binary, :little)
  def bin_to_float32(binary), do: :binary.decode_float(binary, :little)
  def bin_to_float64(binary), do: :binary.decode_float(binary, :little)

  def bin_to_string(binary), do: binary |> :binary.bin_to_list() |> Enum.filter(&(&1 != 0)) |> :binary.list_to_bin()

  def int8_to_bin(arg), do: <<arg::signed-size(8)-little>>
  def int16_to_bin(arg), do: <<arg::signed-size(16)-little>>
  def int32_to_bin(arg), do: <<arg::signed-size(32)-little>>
  def bool_to_bin(arg), do: <<arg::signed-size(32)-little>>
  def int64_to_bin(arg), do: <<arg::signed-size(64)-little>>
  def uint8_to_bin(arg), do: <<arg::unsigned-size(8)-little>>
  def uint16_to_bin(arg), do: <<arg::unsigned-size(16)-little>>
  def uint32_to_bin(arg), do: <<arg::unsigned-size(32)-little>>
  def uint64_to_bin(arg), do: <<arg::unsigned-size(64)-little>>
  def float32_to_bin(arg), do: :binary.encode_float(arg, :little)
  def float64_to_bin(arg), do: :binary.encode_float(arg, :little)
  def string_to_bin(arg), do: :binary.list_to_bin(:binary.bin_to_list(arg) ++ [0])

  def type_to_bin("int8", arg), do: int8_to_bin(arg)
  def type_to_bin("uint8", arg), do: uint8_to_bin(arg)
  def type_to_bin("int16", arg), do: int16_to_bin(arg)
  def type_to_bin("uint16", arg), do: uint16_to_bin(arg)
  def type_to_bin("int32", arg), do: int32_to_bin(arg)
  def type_to_bin("bool", arg), do: bool_to_bin(arg)
  def type_to_bin("uint32", arg), do: uint32_to_bin(arg)
  def type_to_bin("int64", arg), do: int64_to_bin(arg)
  def type_to_bin("uint64", arg), do: uint64_to_bin(arg)
  def type_to_bin("float32", arg), do: float32_to_bin(arg)
  def type_to_bin("float64", arg), do: float64_to_bin(arg)
  def type_to_bin("string", arg), do: string_to_bin(arg)
  def type_to_bin(_type, _arg), do: nil

  def bin_to_value("int8", arg), do: bin_to_int8(arg)
  def bin_to_value("uint8", arg), do: bin_to_uint8(arg)
  def bin_to_value("int16", arg), do: bin_to_int16(arg)
  def bin_to_value("uint16", arg), do: bin_to_uint16(arg)
  def bin_to_value("int32", arg), do: bin_to_int32(arg)
  def bin_to_value("bool", arg), do: bin_to_bool(arg)
  def bin_to_value("uint32", arg), do: bin_to_uint32(arg)
  def bin_to_value("int64", arg), do: bin_to_int64(arg)
  def bin_to_value("uint64", arg), do: bin_to_uint64(arg)
  def bin_to_value("float32", arg), do: bin_to_float32(arg)
  def bin_to_value("float64", arg), do: bin_to_float64(arg)
  def bin_to_value("string", arg), do: bin_to_string(arg)
  def bin_to_value(_type, _arg), do: nil

  def bin_to_array_values(type, arg) do
    case type do
      "int8" -> for <<x::signed-8 <- arg>>, do: x
      "uint8" -> for <<x::unsigned-8 <- arg>>, do: x
      "int16" -> for <<x::signed-16-little <- arg>>, do: x
      "uint16" -> for <<x::unsigned-16-little <- arg>>, do: x
      "int32" -> for <<x::signed-32-little <- arg>>, do: x
      "bool" -> for <<x::signed-32-little <- arg>>, do: x
      "uint32" -> for <<x::unsigned-32-little <- arg>>, do: x
      "int64" -> for <<x::signed-64-little <- arg>>, do: x
      "uint64" -> for <<x::unsigned-64-little <- arg>>, do: x
      "float32" -> for <<x::float-32-little <- arg>>, do: x
      "float64" -> for <<x::float-64-little <- arg>>, do: x
      _ -> nil
    end
  end

  def write_binary(binary_data, off, bin) do
    binary_data =
      Enum.with_index(bin)
      |> Enum.reduce(binary_data, fn {data, i}, acc ->
        <<before::binary-size(off + i), _::binary>> = acc
        <<before::binary, data::binary, acc::binary>>
      end)

    binary_data
  end

  def read_binary(binary_data, off, size) do
    binary_part(binary_data, off, size)
  end
end

defmodule PduMetaData do
  @pdu_meta_data_size 24
  @pdu_meta_data_magicno 0x12345678
  @pdu_meta_data_version 1

  defstruct [:magicno, :version, :base_off, :heap_off, :total_size]

  def new do
    %PduMetaData{
      magicno: @pdu_meta_data_magicno,
      version: @pdu_meta_data_version,
      base_off: @pdu_meta_data_size,
      heap_off: 0,
      total_size: 0
    }
  end

  def to_bytes(meta) do
    <<meta.magicno::32-little, meta.version::32-little, meta.base_off::32-little,
      meta.heap_off::32-little, meta.total_size::32-little>>
  end
end

defmodule PduMetaDataParser do
  def load_pdu_meta(binary_data) do
    if byte_size(binary_data) < PduMetaData.get_pdu_meta_data_size() do
      {:error, :invalid_data}
    else
      <<magicno::32-little, version::32-little, base_off::32-little,
        heap_off::32-little, total_size::32-little>> = binary_data

      if magicno != PduMetaData.get_pdu_meta_data_magicno() or version != PduMetaData.get_pdu_meta_data_version() do
        {:error, :invalid_magicno_or_version}
      else
        {:ok,
         %PduMetaData{
           magicno: magicno,
           version: version,
           base_off: base_off,
           heap_off: heap_off,
           total_size: total_size
         }}
      end
    end
  end
end
