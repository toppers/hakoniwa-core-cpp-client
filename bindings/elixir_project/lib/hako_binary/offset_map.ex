defmodule OffsetMap do
  defstruct off_path: nil, map: %{}

  def create_offmap(offset_path) do
    %OffsetMap{off_path: offset_path}
  end

  def get(%OffsetMap{off_path: off_path, map: map} = state, typename) do
    case Map.has_key?(map, typename) do
      true ->
        {Map.get(map, typename), state}

      false ->
        filepath = find_filepath("#{off_path}/*/", "#{typename}.offset")
        lines = OffsetParser.parse_offset(filepath)
        updated_map = Map.put(map, typename, lines)
        {%{state | map: updated_map}, updated_map[typename]}
    end
  end

  defp find_filepath(path, filename) do
    filepath_parts = String.split(filename, "/")
    filename = List.last(filepath_parts)

    tmp = Path.wildcard("#{path}#{filename}")

    if tmp == [] do
      IO.puts("ERROR: find_filepath(#{path}, #{filename})")
      System.halt(1)
    else
      List.first(tmp)
    end
  end
end
