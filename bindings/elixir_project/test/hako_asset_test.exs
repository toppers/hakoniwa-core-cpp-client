defmodule HakoAssetTest do
  use ExUnit.Case
  doctest HakoAsset

  test "initializes the asset" do
    assert HakoAsset.init_asset() == :true
  end
end
