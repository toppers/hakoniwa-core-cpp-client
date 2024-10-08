defmodule HakoBinary do
  alias PduMetaData
  alias OffsetParser

  def decode_base64(data) do
    :base64.decode(data)
  end

  def binary_read(offmap, typename, binary_data) do
    json_data = %{}
    meta = PduMetaDataParser.load_pdu_meta(binary_data)

    meta =
      case meta do
        nil ->
          meta = PduMetaData.new()
          PduMetaData.set_empty(meta)
          PduConversion.write_binary(binary_data, 0, PduMetaData.to_bytes(meta))
          meta

        _ -> meta
      end

    binary_read_recursive(meta, offmap, binary_data, json_data, PduMetaData.PDU_META_DATA_SIZE, typename)
    json_data
  end

  defp binary_read_recursive(meta, offmap, binary_data, json_data, base_off, typename) do
    lines = OffsetMap.get(offmap, typename)

    for line <- lines do
      off = OffsetParser.member_off(line) + base_off
      type = OffsetParser.member_type(line)
      name = OffsetParser.member_name(line)
      size = OffsetParser.member_size(line)

      cond do
        OffsetParser.is_primitive(line) ->
          cond do
            OffsetParser.is_single(line) ->
              bin = PduConversion.read_binary(binary_data, off, size)
              value = PduConversion.bin_to_value(type, bin)
              Map.put(json_data, name, value)

            OffsetParser.is_array(line) ->
              array_value = PduConversion.read_binary(binary_data, off, size)
              json_data = Map.put(json_data, "#{name}__raw", array_value)
              Map.put(json_data, name, PduConversion.bin_to_array_values(type, array_value))

            true ->  # varray
              array_size = PduConversion.bin_to_value("int32", PduConversion.read_binary(binary_data, off, 4))
              offset_from_heap = PduConversion.bin_to_value("int32", PduConversion.read_binary(binary_data, off + 4, 4))
              one_elm_size = size
              array_value = PduConversion.read_binary(binary_data, meta.heap_off + offset_from_heap, one_elm_size * array_size)
              json_data = Map.put(json_data, "#{name}__raw", array_value)
              Map.put(json_data, name, PduConversion.bin_to_array_values(type, array_value))
          end

        true -> # nested or array of structs
          cond do
            OffsetParser.is_single(line) ->
              tmp_json_data = %{}
              binary_read_recursive(meta, offmap, binary_data, tmp_json_data, off, type)
              Map.put(json_data, name, tmp_json_data)

            OffsetParser.is_array(line) ->
              array_size = OffsetParser.array_size(line)
              one_elm_size = div(size, array_size)
              array_value = for i <- 0..(array_size - 1) do
                tmp_json_data = %{}
                binary_read_recursive(meta, offmap, binary_data, tmp_json_data, off + (i * one_elm_size), type)
                tmp_json_data
              end
              Map.put(json_data, name, array_value)

            true ->  # varray
              array_size = PduConversion.bin_to_value("int32", PduConversion.read_binary(binary_data, off, 4))
              offset_from_heap = PduConversion.bin_to_value("int32", PduConversion.read_binary(binary_data, off + 4, 4))
              one_elm_size = size
              array_value = for i <- 0..(array_size - 1) do
                tmp_json_data = %{}
                binary_read_recursive(meta, offmap, binary_data, tmp_json_data, meta.heap_off + offset_from_heap + (i * one_elm_size), type)
                tmp_json_data
              end
              Map.put(json_data, name, array_value)
          end
      end
    end
    json_data
  end
end
