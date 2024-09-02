defmodule HakoAssetTest do
  use ExUnit.Case

  alias HakoAsset

  setup do
    HakoAsset.start_link(nil)
    :ok
  end

  test "register asset with valid callbacks and parameters" do
    asset_name = "test_asset"
    config_path = Path.join([__DIR__, "config/custom.json"])
    delta_usec = 10000
    max_delay_usec = 20000
    model_type = :controller
    callbacks = %{
      on_initialize: fn _ -> 0 end,
      on_simulation_step: fn _ -> 0 end,
      on_manual_timing_control: nil,  # これをnilに設定
      on_reset: fn _ -> 0 end
    }
    assert HakoConductor.start(delta_usec, max_delay_usec) == :ok
    assert HakoAsset.register(asset_name, config_path, callbacks, delta_usec, model_type) == :ok
    assert HakoConductor.stop() == :ok
  end
end
