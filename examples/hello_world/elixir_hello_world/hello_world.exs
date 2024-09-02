alias HakoAsset
alias HakoConductor
Code.append_path("../../../bindings/elixir_project/_build/dev/lib/elixir_project/ebin")

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
    IO.puts("INFO: sleep 1sec: #{HakoAsset.simulation_time()}")
    result = HakoAsset.usleep(1000)
    :timer.sleep(1000)  # 1秒スリープ
    if result == :ok do
      loop_until_wakeup()
    else
      result
    end
  end


  def main(args) do
    IO.inspect(args, label: "Received args")

    HakoAsset.start_link(nil)
    case args do
      [asset_name, config_path, delta_time_msec] ->
        Logger.info("INFO: 自動タイミングモードで実行中")
        delta_time_usec = String.to_integer(delta_time_msec) * 1000
        HakoConductor.start(delta_time_usec, delta_time_usec)

        callbacks = %{
          on_initialize: &HelloWorld.my_on_initialize/1,
          on_manual_timing_control: nil,
          on_simulation_step: &HelloWorld.my_on_simulation_step/1,
          on_reset: &HelloWorld.my_on_reset/1
        }
        # Register the asset with the appropriate callbacks
        case HakoAsset.register(asset_name, config_path, callbacks, delta_time_usec, :controller) do
          :false ->
            Logger.error("ERROR: hako_asset_register() が #{:false} を返しました。")
            HakoConductor.stop()
            System.halt(1)
          _ ->
            :ok
        end

      [asset_name, config_path, delta_time_msec, "manual"] ->
        Logger.info("INFO: 手動タイミングモードで実行中")
        delta_time_usec = String.to_integer(delta_time_msec) * 1000
        HakoConductor.start(delta_time_usec, delta_time_usec)

        callbacks = %{
          on_initialize: &HelloWorld.my_on_initialize/1,
          on_manual_timing_control: &HelloWorld.my_on_manual_timing_control/1,
          on_simulation_step: nil,
          on_reset: &HelloWorld.my_on_reset/1
        }
        # Register the asset with the appropriate callbacks
        case HakoAsset.register(asset_name, config_path, callbacks, delta_time_usec, :controller) do
          :false ->
            Logger.error("ERROR: hako_asset_register() が #{:false} を返しました。")
            HakoConductor.stop()
            System.halt(1)
          _ ->
            :ok
        end
    Logger.info("INFO: HakoAsset.asset_register() completed successfully")

      _ ->
        Logger.error("ERROR: Invalid arguments. Expected: [asset_name, config_path, delta_time_msec] or [asset_name, config_path, delta_time_msec, manual]")
        System.halt(1)
    end


    result = HakoAsset.start()
    case result do
      {:ok, _} ->
        Logger.info("INFO: hako_asset_start() completed successfully")

      {:error, reason} ->
        Logger.error("ERROR: hako_asset_start() failed with reason: #{inspect(reason)}")

      _ ->
        Logger.error("ERROR: Unexpected return value from hako_asset_start(): #{inspect(result)}")
    end

    HakoConductor.stop()
  end
end

args = System.argv()
HelloWorld.main(args)
