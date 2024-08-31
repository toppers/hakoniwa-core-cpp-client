defmodule HakoAsset do
  @on_load :load_nif

  def load_nif do
    app_name = :elixir_hello_world
    priv_dir = :code.priv_dir(app_name)
    nif_path = Path.join(priv_dir, "libhako_asset_nif")
    :erlang.load_nif(nif_path, 0)
  end

  # NIF関数のプレースホルダー
  def init_asset, do: :erlang.nif_error(:nif_not_loaded)
  def asset_register(_name, _config_path, _callbacks, _delta_usec, _model), do: :erlang.nif_error(:nif_not_loaded)
  def conductor_start(_delta_usec, _max_delay_usec), do: :erlang.nif_error(:nif_not_loaded)
  def conductor_stop, do: :erlang.nif_error(:nif_not_loaded)
  def start, do: :erlang.nif_error(:nif_not_loaded)
  def usleep(_microseconds), do: :erlang.nif_error(:nif_not_loaded)
  def simulation_time, do: :erlang.nif_error(:nif_not_loaded)
  def pdu_read(_robo_name, _channel_id, _buffer_len), do: :erlang.nif_error(:nif_not_loaded)
  def pdu_write(_robo_name, _channel_id, _pdu_data), do: :erlang.nif_error(:nif_not_loaded)
  def init_for_external, do: :erlang.nif_error(:nif_not_loaded)
end
