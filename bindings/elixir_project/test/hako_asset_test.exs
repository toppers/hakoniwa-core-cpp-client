defmodule HakoApiTest do
  use ExUnit.Case
  doctest HakoApiTest

  test "initializes the asset" do
    assert HakoApi.init_asset() == :true
  end
end
