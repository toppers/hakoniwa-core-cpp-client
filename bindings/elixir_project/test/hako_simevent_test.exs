defmodule HakoSimeventTest do
  use ExUnit.Case
  #import HakoConductor
  doctest HakoSimevent

  test "initializes the simulation event" do
    assert HakoConductor.start(1000, 2000) == :ok
    assert HakoSimevent.init() == :true
    assert HakoSimevent.get_state() == :stopped
    assert HakoSimevent.start() == :true
    assert HakoSimevent.get_state() == :runnable
    assert HakoConductor.stop() == :ok
  end

end
