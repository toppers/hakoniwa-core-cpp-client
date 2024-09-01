defmodule HakoConductor do
  @moduledoc """
  HakoConductorは、箱庭システムのコンダクター機能を操作するためのモジュールです。
  """

  @doc """
  コンダクターを開始します。

  ## Parameters
    - `delta_usec`: シミュレーションの更新間隔をマイクロ秒で指定します。
    - `max_delay_usec`: 最大許容遅延時間をマイクロ秒で指定します。

  ## Returns
    - `:ok` 成功した場合
    - `:error` 失敗した場合
  """
  def start(delta_usec, max_delay_usec) do
    case HakoApi.conductor_start(delta_usec, max_delay_usec) do
      :true -> :ok
      :false -> :error
    end
  end

  @doc """
  コンダクターを停止します。

  ## Returns
    - `:ok` 成功した場合
  """
  def stop do
    HakoApi.conductor_stop()
  end
end
