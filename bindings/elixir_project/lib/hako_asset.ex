defmodule HakoAsset do
  @on_load :load_nif

  def load_nif do
    :ok = :erlang.load_nif("priv/libhako_asset_nif", 0)
  end

  # NIF関数のプレースホルダー
  def init_asset(), do: :erlang.nif_error(:nif_not_loaded)
  def register_asset(_name), do: :erlang.nif_error(:nif_not_loaded)
  def get_event(_name), do: :erlang.nif_error(:nif_not_loaded)
  def unregister_asset(_name), do: :erlang.nif_error(:nif_not_loaded)
  def notify_simtime(_name, _simtime), do: :erlang.nif_error(:nif_not_loaded)
  def get_worldtime(), do: :erlang.nif_error(:nif_not_loaded)
  def create_pdu_channel(_channel_id, _pdu_size), do: :erlang.nif_error(:nif_not_loaded)
  def create_pdu_lchannel(_robo_name, _channel_id, _pdu_size), do: :erlang.nif_error(:nif_not_loaded)
  def get_pdu_channel(_robo_name, _channel_id), do: :erlang.nif_error(:nif_not_loaded)
  def is_pdu_dirty(_asset_name, _robo_name, _channel_id), do: :erlang.nif_error(:nif_not_loaded)
  def write_pdu(_asset_name, _robo_name, _channel_id, _pdu_data), do: :erlang.nif_error(:nif_not_loaded)
  def read_pdu(_asset_name, _robo_name, _channel_id, _pdu_size), do: :erlang.nif_error(:nif_not_loaded)
  def write_pdu_nolock(_robo_name, _channel_id, _pdu_data), do: :erlang.nif_error(:nif_not_loaded)
  def read_pdu_nolock(_robo_name, _channel_id, _pdu_size), do: :erlang.nif_error(:nif_not_loaded)
  def start_feedback(_asset_name, _isOk), do: :erlang.nif_error(:nif_not_loaded)
  def stop_feedback(_asset_name, _isOk), do: :erlang.nif_error(:nif_not_loaded)
  def reset_feedback(_asset_name, _isOk), do: :erlang.nif_error(:nif_not_loaded)
  def notify_read_pdu_done(_asset_name), do: :erlang.nif_error(:nif_not_loaded)
  def notify_write_pdu_done(_asset_name), do: :erlang.nif_error(:nif_not_loaded)
  def is_pdu_sync_mode(_asset_name), do: :erlang.nif_error(:nif_not_loaded)
  def is_simulation_mode(), do: :erlang.nif_error(:nif_not_loaded)
  def is_pdu_created(), do: :erlang.nif_error(:nif_not_loaded)
  def simevent_init(), do: :erlang.nif_error(:nif_not_loaded)
  def simevent_get_state(), do: :erlang.nif_error(:nif_not_loaded)
  def simevent_start(), do: :erlang.nif_error(:nif_not_loaded)
  def simevent_stop(), do: :erlang.nif_error(:nif_not_loaded)
  def simevent_reset(), do: :erlang.nif_error(:nif_not_loaded)
  def conductor_start(_delta_usec, _max_delay_usec), do: :erlang.nif_error(:nif_not_loaded)
  def conductor_stop(), do: :erlang.nif_error(:nif_not_loaded)
end
