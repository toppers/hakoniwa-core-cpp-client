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
  """
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
    #TODO
  end

  @doc """
  Reads PDU data from the specified robot and channel.
  """
  def pdu_read(robo_name, channel_id, buffer_len) do
    #TODO
  end

  @doc """
  Writes PDU data to the specified robot and channel.
  """
  def pdu_write(robo_name, channel_id, buffer) do
    #TODO
  end

  @doc """
  Retrieves the current simulation time.
  """
  def simulation_time() do
    #TODO
  end

  @doc """
  Puts the asset to sleep for the specified amount of time.
  """
  def usleep(sleep_time_usec) do
    #TODO
  end

end
