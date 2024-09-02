defmodule HakoAsset do
  @moduledoc """
  HakoAsset is the front module for managing Hakoniwa assets.
  It delegates the core functionality to HakoAssetImpl.
  """

  @type context :: nil | any()
  @type model_type :: :plant | :controller
  @type asset_name :: String.t()
  @type config_path :: String.t()
  @type delta_usec :: non_neg_integer()
  @type buffer :: binary()
  @type buffer_len :: non_neg_integer()
  @type pdu_channel_id :: non_neg_integer()

  @type callbacks :: %{
    on_initialize: (context() -> integer()),
    on_simulation_step: (context() -> integer()),
    on_manual_timing_control: (context() -> integer()),
    on_reset: (context() -> integer())
  }

  alias HakoAssetImpl

  # Agentの初期化
  def start_link(_) do
    HakoAssetImpl.start_link(nil)
  end

  # API
  @doc """
  Initializes the module for external use.
  Sets the flag to indicate external initialization.
  """
  def initialize_for_external() do
    HakoAssetImpl.set_external_use(true)
    :ok
  end

  @doc """
  Checks if the module is initialized for external use.
  """
  def is_external? do
    HakoAssetImpl.get_external_use()
  end

  @doc """
  Registers an asset with the specified callbacks and configuration.
  Returns {:ok, :registered} on success, or {:error, reason} on failure.
  """
  def register(asset_name, config_path, callbacks, delta_usec, model_type) do
    cond do
      asset_name == nil or asset_name == "" ->
        {:error, "Asset name is not set."}

      config_path == nil or !File.exists?(config_path) ->
        {:error, "Config file does not exist."}

      callbacks == nil ->
        {:error, "Callbacks are not set."}

      callbacks.on_simulation_step != nil and callbacks.on_manual_timing_control != nil ->
        {:error, "Both on_simulation_step and on_manual_timing_control are set."}

      delta_usec < 1000 ->
        {:error, "Delta_usec (#{delta_usec}) is too small. Minimum is 1000."}

      true ->
        HakoAssetImpl.register_callback(callbacks)

        case HakoAssetImpl.init(asset_name, config_path, delta_usec, model_type == :plant) do
          :ok ->
            IO.puts("INFO: asset(#{asset_name}) is registered.")
            :ok
          {:error, reason} ->
            # エラー時の処理
            {:error, reason}
        end
    end
  end

  @doc """
  Starts the asset management process.
  """

  def start() do
    asset_instance = HakoAssetImpl.get_asset_instance()

    # 初期化がされていない場合はエラー
    unless asset_instance.is_initialized do
      IO.puts("Error: not initialized.")
      {:error, :not_initialized}
    else
      # シミュレーション状態が停止状態でない場合はエラー
      if HakoSimevent.get_state() != :stopped do
        IO.puts("Error: simulation state is invalid, expecting stopped.")
        {:error, :invalid_state}
      else
        # シミュレーションの実行を待機
        case HakoAssetImpl.wait_running() do
          true ->
            IO.puts("INFO: start simulation")

            # manual_timing_controlが設定されていれば実行
            if asset_instance.callback && asset_instance.callback.on_manual_timing_control do
              asset_instance.callback.on_manual_timing_control(nil)
            else
              # シミュレーションステップを実行し続ける
              loop_simulation()
            end

          false ->
            IO.puts("Error: can not wait running for start.")
            {:error, :wait_running_failed}
        end
      end
    end
  end

  # シミュレーションステップを実行し続ける
  defp loop_simulation() do
    case HakoAssetImpl.step(1) do
      true ->
        loop_simulation()

      false ->
        IO.puts("INFO: stopped simulation")
        {:error, :simulation_stopped}
    end
  end

  # PDUデータの読み込み
  @doc """
  Reads PDU data from the specified robot and channel.
  """
  def pdu_read(robo_name, lchannel, buffer_len) when is_binary(robo_name) and is_integer(lchannel) and is_integer(buffer_len) do
    if !HakoAssetImpl.is_initialized?() do
      {:error, "Error: not initialized."}
    else
      cond do
        robo_name == "" ->
          {:error, "Error: robo_name is not set."}

        buffer_len <= 0 ->
          {:error, "Error: Invalid buffer or buffer_len."}

        true ->
          case HakoAssetImpl.pdu_read(robo_name, lchannel, buffer_len) do
            {:ok, buffer} -> {:ok, buffer}
            :error -> {:error, "Error: Failed to read PDU data."}
          end
      end
    end
  end

  # PDUデータの書き込み
  @doc """
  Writes PDU data to the specified robot and channel.
  """
  def pdu_write(robo_name, lchannel, buffer) when is_binary(robo_name) and is_integer(lchannel) and is_binary(buffer) do
    if !HakoAssetImpl.is_initialized?() do
      {:error, "Error: not initialized."}
    else
      cond do
        robo_name == "" ->
          {:error, "Error: robo_name is not set."}

        byte_size(buffer) == 0 ->
          {:error, "Error: Invalid buffer or buffer_len."}

        true ->
          case HakoAssetImpl.pdu_write(robo_name, lchannel, buffer) do
            :ok -> :ok
            :error -> {:error, "Error: Failed to write PDU data."}
          end
      end
    end
  end

  # シミュレーション時間の取得
  @doc """
  Retrieves the current simulation time.
  """
  def simulation_time() do
    HakoAssetImpl.get_world_time()
  end

  # 指定された時間だけスリープする
  @doc """
  Puts the asset to sleep for the specified amount of time in microseconds.
  """
  def usleep(sleep_time_usec) when is_integer(sleep_time_usec) and sleep_time_usec >= 0 do
    step =
      if sleep_time_usec == 0 do
        1
      else
        delta_usec = HakoAssetImpl.get_delta_usec()
        div(sleep_time_usec + delta_usec - 1, delta_usec)
      end

    case HakoAssetImpl.step(step) do
      :ok -> :ok
      :error -> :eintr
    end
  end

end
