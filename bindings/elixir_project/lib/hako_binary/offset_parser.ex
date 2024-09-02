defmodule OffsetParser do
  def parse_offset(filepath) do
    filepath
    |> File.read!()
    |> String.split("\n", trim: true)
  end

  def select_by_name(lines, name) do
    Enum.find(lines, fn line -> member_name(line) == name end)
  end

  def member_name(data) do
    data
    |> String.split(":")
    |> Enum.at(2)
  end

  def member_type(data) do
    data
    |> String.split(":")
    |> Enum.at(3)
  end

  def is_primitive(data) do
    data
    |> String.split(":")
    |> Enum.at(1)
    |> String.trim() == "primitive"
  end

  def is_single(data) do
    data
    |> String.split(":")
    |> Enum.at(0)
    |> String.trim() == "single"
  end

  def is_array(data) do
    data
    |> String.split(":")
    |> Enum.at(0)
    |> String.trim() == "array"
  end

  def is_varray(data) do
    data
    |> String.split(":")
    |> Enum.at(0)
    |> String.trim() == "varray"
  end

  def array_size(data) do
    cond do
      is_single(data) -> 0
      is_varray(data) -> -1
      true ->
        data
        |> String.split(":")
        |> Enum.at(6)
        |> String.to_integer()
    end
  end

  def member_off(data) do
    data
    |> String.split(":")
    |> Enum.at(4)
    |> String.to_integer()
  end

  def member_size(data) do
    data
    |> String.split(":")
    |> Enum.at(5)
    |> String.to_integer()
  end
end
