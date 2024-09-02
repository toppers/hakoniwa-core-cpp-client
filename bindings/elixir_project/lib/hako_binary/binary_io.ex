defmodule PduConversion do
  def bin_to_int8(<<value::signed-8>>), do: value
  def bin_to_int16(<<value::signed-16-little>>), do: value
  def bin_to_int32(<<value::signed-32-little>>), do: value
  def bin_to_bool(<<value::signed-32-little>>), do: value
  def bin_to_int64(<<value::signed-64-little>>), do: value
  def bin_to_uint8(<<value::unsigned-8>>), do: value
  def bin_to_uint16(<<value::unsigned-16-little>>), do: value
  def bin_to_uint32(<<value::unsigned-32-little>>), do: value
  def bin_to_uint64(<<value::unsigned-64-little>>), do: value
  def bin_to_float32(<<value::float-32-little>>), do: value
  def bin_to_float64(<<value::float-64-little>>), do: value

  def bin_to_string(binary), do: binary |> :binary.bin_to_list() |> Enum.filter(&(&1 != 0)) |> :binary.list_to_bin()

  def int8_to_bin(value), do: <<value::signed-8>>
  def int16_to_bin(value), do: <<value::signed-16-little>>
  def int32_to_bin(value), do: <<value::signed-32-little>>
  def bool_to_bin(value), do: <<value::signed-32-little>>
  def int64_to_bin(value), do: <<value::signed-64-little>>
  def uint8_to_bin(value), do: <<value::unsigned-8>>
  def uint16_to_bin(value), do: <<value::unsigned-16-little>>
  def uint32_to_bin(value), do: <<value::unsigned-32-little>>
  def uint64_to_bin(value), do: <<value::unsigned-64-little>>
  def float32_to_bin(value), do: <<value::float-32-little>>
  def float64_to_bin(value), do: <<value::float-64-little>>
  def string_to_bin(value), do: :binary.list_to_bin(:binary.bin_to_list(value) ++ [0])

  def type_to_bin("int8", value), do: int8_to_bin(value)
  def type_to_bin("uint8", value), do: uint8_to_bin(value)
  def type_to_bin("int16", value), do: int16_to_bin(value)
  def type_to_bin("uint16", value), do: uint16_to_bin(value)
  def type_to_bin("int32", value), do: int32_to_bin(value)
  def type_to_bin("bool", value), do: bool_to_bin(value)
  def type_to_bin("uint32", value), do: uint32_to_bin(value)
  def type_to_bin("int64", value), do: int64_to_bin(value)
  def type_to_bin("uint64", value), do: uint64_to_bin(value)
  def type_to_bin("float32", value), do: float32_to_bin(value)
  def type_to_bin("float64", value), do: float64_to_bin(value)
  def type_to_bin("string", value), do: string_to_bin(value)
  def type_to_bin(_type, _value), do: nil

  def bin_to_value("int8", value), do: bin_to_int8(value)
  def bin_to_value("uint8", value), do: bin_to_uint8(value)
  def bin_to_value("int16", value), do: bin_to_int16(value)
  def bin_to_value("uint16", value), do: bin_to_uint16(value)
  def bin_to_value("int32", value), do: bin_to_int32(value)
  def bin_to_value("bool", value), do: bin_to_bool(value)
  def bin_to_value("uint32", value), do: bin_to_uint32(value)
  def bin_to_value("int64", value), do: bin_to_int64(value)
  def bin_to_value("uint64", value), do: bin_to_uint64(value)
  def bin_to_value("float32", value), do: bin_to_float32(value)
  def bin_to_value("float64", value), do: bin_to_float64(value)
  def bin_to_value("string", value), do: bin_to_string(value)
  def bin_to_value(_type, _value), do: nil

  def bin_to_array_values("int8", value), do: for(<<x::signed-8 <- value>>, do: x)
  def bin_to_array_values("uint8", value), do: for(<<x::unsigned-8 <- value>>, do: x)
  def bin_to_array_values("int16", value), do: for(<<x::signed-16-little <- value>>, do: x)
  def bin_to_array_values("uint16", value), do: for(<<x::unsigned-16-little <- value>>, do: x)
  def bin_to_array_values("int32", value), do: for(<<x::signed-32-little <- value>>, do: x)
  def bin_to_array_values("bool", value), do: for(<<x::signed-32-little <- value>>, do: x)
  def bin_to_array_values("uint32", value), do: for(<<x::unsigned-32-little <- value>>, do: x)
  def bin_to_array_values("int64", value), do: for(<<x::signed-64-little <- value>>, do: x)
  def bin_to_array_values("uint64", value), do: for(<<x::unsigned-64-little <- value>>, do: x)
  def bin_to_array_values("float32", value), do: for(<<x::float-32-little <- value>>, do: x)
  def bin_to_array_values("float64", value), do: for(<<x::float-64-little <- value>>, do: x)

  def bin_to_array_values(_type, _value), do: nil

  def write_binary(binary_data, off, bin) do
    Enum.with_index(bin)
    |> Enum.reduce(binary_data, fn {data, i}, acc ->
      <<before::binary-size(off + i), _::binary>> = acc
      <<before::binary, data::binary, acc::binary>>
    end)
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
      magicno: 0,
      version: 0,
      base_off: 0,
      heap_off: 0,
      total_size: 0
    }
  end

  def set_empty(%PduMetaData{} = meta) do
    %PduMetaData{
      meta
      | magicno: @pdu_meta_data_magicno,
        version: @pdu_meta_data_version,
        base_off: @pdu_meta_data_size
    }
  end

  def to_bytes(%PduMetaData{} = meta) do
    <<meta.magicno::32-little, meta.version::32-little, meta.base_off::32-little,
      meta.heap_off::32-little, meta.total_size::32-little>>
  end

  def get_pdu_meta_data_size, do: @pdu_meta_data_size
  def get_pdu_meta_data_magicno, do: @pdu_meta_data_magicno
  def get_pdu_meta_data_version, do: @pdu_meta_data_version
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
