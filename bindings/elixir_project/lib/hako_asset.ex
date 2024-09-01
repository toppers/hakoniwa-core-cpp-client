defmodule HakoAsset do
  @moduledoc """
  Provides an interface for the Hakoniwa asset management.
  """

  @type asset_name :: String.t()
  @type config_path :: String.t()
  @type delta_usec :: non_neg_integer()
  @type buffer :: binary()
  @type buffer_len :: non_neg_integer()
  @type pdu_channel_id :: non_neg_integer()
  @type model_type :: :plant | :controller
  @type context :: any()

  @type callbacks :: %{
          on_initialize: (context() -> integer()),
          on_simulation_step: (context() -> integer()),
          on_manual_timing_control: (context() -> integer()),
          on_reset: (context() -> integer())
        }

  @callback register(asset_name(), config_path(), callbacks(), delta_usec(), model_type()) :: :ok | :error
  @callback start() :: :ok | :error
  @callback pdu_read(String.t(), pdu_channel_id(), buffer_len()) :: {:ok, buffer()} | :error
  @callback pdu_write(String.t(), pdu_channel_id(), buffer()) :: :ok | :error
  @callback simulation_time() :: non_neg_integer()
  @callback usleep(delta_usec()) :: :ok | :error
end
