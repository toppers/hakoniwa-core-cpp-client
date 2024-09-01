defmodule HakoSimevent do
  @moduledoc """
  HakoSimevent module provides an interface for managing simulation events.
  """

  # Define simulation state mappings
  @simulation_states %{
    0 => :stopped,
    1 => :runnable,
    2 => :running,
    3 => :stopping,
    4 => :resetting,
    5 => :error,
    6 => :terminated,
    7 => :any
  }

  @doc """
  Initialize the simulation event.
  """
  def init do
    HakoApi.simevent_init()
  end

  @doc """
  Get the current simulation state.
  """
  def get_state do
    state_code = HakoApi.simevent_get_state()
    Map.get(@simulation_states, state_code, :unknown)
  end

  @doc """
  Start the simulation event.
  """
  def start do
    HakoApi.simevent_start()
  end

  @doc """
  Stop the simulation event.
  """
  def stop do
    HakoApi.simevent_stop()
  end

  @doc """
  Reset the simulation event.
  """
  def reset do
    HakoApi.simevent_reset()
  end
end
