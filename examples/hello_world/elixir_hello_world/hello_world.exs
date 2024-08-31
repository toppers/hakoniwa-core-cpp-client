alias HakoAsset

defmodule HelloWorld do
  require Logger

  def my_on_initialize(_context) do
    Logger.info("INFO: my_on_initialize enter")
    :timer.sleep(1000)  # 1秒スリープ
    Logger.info("INFO: my_on_initialize exit")
    0
  end

  def my_on_reset(_context) do
    Logger.info("INFO: my_on_reset enter")
    :timer.sleep(1000)  # 1秒スリープ
    Logger.info("INFO: my_on_reset exit")
    0
  end

  def my_on_simulation_step(_context) do
    Logger.info("INFO: on_simulation_step enter: #{HakoAsset.simulation_time()}")
    Logger.info("INFO: sleep 1sec")
    :timer.sleep(1000)  # 1秒スリープ
    Logger.info("INFO: on_simulation_step exit")
    0
  end

  def my_on_manual_timing_control(_context) do
    Logger.info("INFO: on_manual_timing_control enter")
    result = loop_until_wakeup()
    Logger.info("INFO: on_manual_timing_control exit")
    result
  end

  defp loop_until_wakeup do
    result = HakoAsset.usleep(1000)
    :timer.sleep(1000)  # 1秒スリープ
    if result == 0 do
      loop_until_wakeup()
    else
      result
    end
  end

  def main(args) do
    case args do
      [asset_name, config_path, delta_time_msec] ->
        Logger.info("INFO: 自動タイミングモードで実行中")
        delta_time_usec = String.to_integer(delta_time_msec) * 1000
        HakoAsset.conductor_start(delta_time_usec, delta_time_usec)
        callbacks = %{
          on_initialize: &HelloWorld.my_on_initialize/1,
          on_manual_timing_control: nil,
          on_simulation_step: &HelloWorld.my_on_simulation_step/1,
          on_reset: &HelloWorld.my_on_reset/1
        }
        case HakoAsset.asset_register(asset_name, config_path, callbacks, delta_time_usec, :controller) do
          :false ->
            Logger.error("ERROR: hako_asset_register() が #{:false} を返しました。")
            HakoAsset.conductor_stop()
            System.halt(1)
          _ ->
            :ok
        end
        case HakoAsset.start() do
          ret ->
            Logger.info("INFO: hako_asset_start() が #{ret} を返しました。")
        end
        HakoAsset.conductor_stop()

      [asset_name, config_path, delta_time_msec, "manual"] ->
        Logger.info("INFO: マニュアルタイミングモードで実行中")
        delta_time_usec = String.to_integer(delta_time_msec) * 1000
        HakoAsset.conductor_start(delta_time_usec, delta_time_usec)
        callbacks = %{
          on_initialize: &HelloWorld.my_on_initialize/1,
          on_simulation_step: nil,
          on_manual_timing_control: &HelloWorld.my_on_manual_timing_control/1,
          on_reset: &HelloWorld.my_on_reset/1
        }
        case HakoAsset.register_asset(asset_name, config_path, callbacks, delta_time_usec, :controller) do
          :false ->
            Logger.error("ERROR: hako_asset_register() が #{:false} を返しました。")
            HakoAsset.conductor_stop()
            System.halt(1)
          _ ->
            :ok
        end
        case HakoAsset.start() do
          ret ->
            Logger.info("INFO: hako_asset_start() が #{ret} を返しました。")
        end
        HakoAsset.conductor_stop()

      _ ->
        IO.puts("Usage: elixir hello_world.exs <asset_name> <config_path> <delta_time_msec> [manual]")
    end
  end
end

args = System.argv()
HelloWorld.main(args)
