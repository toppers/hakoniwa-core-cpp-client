defmodule DynamicAllocator do
  defstruct data: <<>>, offset_map: %{}, is_heap: false

  def new(is_heap \\ false) do
    %DynamicAllocator{is_heap: is_heap}
  end

  def add(%DynamicAllocator{data: data, offset_map: offset_map, is_heap: is_heap} = allocator, bytes_data, expected_offset \\ nil, key \\ nil) do
    data =
      if is_heap == false and expected_offset do
        current_size = byte_size(data)
        if current_size < expected_offset do
          padding = <<0::size(expected_offset - current_size)-unit(8)>>
          data <> padding
        else
          data
        end
      else
        data
      end

    offset = byte_size(data)
    new_data = data <> bytes_data

    new_offset_map =
      if key do
        Map.put(offset_map, key, offset)
      else
        offset_map
      end

    %DynamicAllocator{allocator | data: new_data, offset_map: new_offset_map}
  end

  def to_array(%DynamicAllocator{data: data}), do: data

  def size(%DynamicAllocator{data: data}), do: byte_size(data)

  def get_offset(%DynamicAllocator{offset_map: offset_map}, key), do: Map.get(offset_map, key)
end

defmodule BinaryWriterContainer do
  defstruct heap_allocator: DynamicAllocator.new(true), meta: %{}

  def new() do
    meta = BinaryIO.PduMetaData.new()
    BinaryIO.PduMetaData.set_empty(meta)
    %BinaryWriterContainer{meta: meta}
  end
end

defmodule BinaryWriter do
  alias __MODULE__
  alias DynamicAllocator
  alias BinaryWriterContainer

  def binary_write(offmap, binary_data, json_data, typename) do
    base_allocator = DynamicAllocator.new(false)
    bw_container = BinaryWriterContainer.new()

    binary_write_recursive(0, bw_container, offmap, base_allocator, json_data, typename)

    total_size = DynamicAllocator.size(base_allocator) + DynamicAllocator.size(bw_container.heap_allocator) + BinaryIO.PduMetaData.PDU_META_DATA_SIZE
    bw_container.meta = Map.put(bw_container.meta, :total_size, total_size)
    bw_container.meta = Map.put(bw_container.meta, :heap_off, BinaryIO.PduMetaData.PDU_META_DATA_SIZE + DynamicAllocator.size(base_allocator))

    binary_data =
      if byte_size(binary_data) < total_size do
        binary_data <> <<0::size(total_size - byte_size(binary_data))-unit(8)>>
      else
        binary_data |> binary_part(0, total_size)
      end

    binary_data =
      binary_data
      |> BinaryIO.write_binary(0, BinaryIO.PduMetaData.to_bytes(bw_container.meta))
      |> BinaryIO.write_binary(bw_container.meta.base_off, DynamicAllocator.to_array(base_allocator))
      |> BinaryIO.write_binary(bw_container.meta.heap_off, DynamicAllocator.to_array(bw_container.heap_allocator))

    binary_data
  end

  defp binary_write_recursive(parent_off, bw_container, offmap, allocator, json_data, typename) do
    lines = OffsetMap.get(offmap, typename)

    Enum.reduce_while(json_data, allocator, fn {key, value}, acc ->
      line = OffsetParser.select_by_name(lines, key)

      if line do
        type = OffsetParser.member_type(line)
        off = OffsetParser.member_off(line)

        acc =
          cond do
            OffsetParser.is_primitive(line) ->
              cond do
                OffsetParser.is_single(line) ->
                  bin = BinaryIO.type_to_bin(type, value)
                  bin = get_binary(type, bin, OffsetParser.member_size(line))
                  DynamicAllocator.add(acc, bin, parent_off + off)

                OffsetParser.is_array(line) ->
                  process_array(acc, parent_off, off, key, value, type, line)

                true ->  # varray
                  process_varray(acc, parent_off, off, key, value, type, line, bw_container.heap_allocator)
              end

            true ->
              cond do
                OffsetParser.is_single(line) ->
                  binary_write_recursive(parent_off + off, bw_container, offmap, acc, value, type)

                OffsetParser.is_array(line) ->
                  process_nested_array(acc, parent_off, off, key, value, type, line, bw_container)

                true ->  # varray
                  process_varray_nested(0, bw_container, offmap, acc, value, type)
              end
          end

        {:cont, acc}
      else
        {:cont, acc}
      end
    end)
  end

  defp process_array(allocator, parent_off, off, key, value, type, line) do
    Enum.reduce(value, allocator, fn elm, acc ->
      elm_size = OffsetParser.member_size(line)
      array_size = OffsetParser.array_size(line)
      one_elm_size = div(elm_size, array_size)
      bin = BinaryIO.type_to_bin(type, elm)
      bin = get_binary(type, bin, one_elm_size)
      DynamicAllocator.add(acc, bin, parent_off + off + Enum.find_index(value, fn x -> x == elm end) * one_elm_size)
    end)
  end

  defp process_varray(allocator, parent_off, off, key, value, type, line, heap_allocator) do
    Enum.reduce(value, allocator, fn elm, acc ->
      bin = BinaryIO.type_to_bin(type, elm)
      bin = get_binary(type, bin, OffsetParser.member_size(line))
      DynamicAllocator.add(heap_allocator, bin, parent_off + off + Enum.find_index(value, fn x -> x == elm end) * OffsetParser.member_size(line))
    end)
  end

  defp process_nested_array(allocator, parent_off, off, key, value, type, line, bw_container) do
    Enum.reduce(value, allocator, fn elm, acc ->
      elm_size = OffsetParser.member_size(line)
      array_size = OffsetParser.array_size(line)
      one_elm_size = div(elm_size, array_size)
      binary_write_recursive(parent_off + off + Enum.find_index(value, fn x -> x == elm end) * one_elm_size, bw_container, offmap, acc, elm, type)
    end)
  end

  defp process_varray_nested(parent_off, bw_container, offmap, allocator, value, type) do
    Enum.reduce(value, allocator, fn elm, acc ->
      binary_write_recursive(parent_off, bw_container, offmap, DynamicAllocator.to_array(bw_container.heap_allocator), elm, type)
    end)
  end

  defp get_binary(type, bin, elm_size) do
    if type == "string" do
      bin = binary_part(bin, 0, elm_size)
    else
      bin
    end
  end
end
