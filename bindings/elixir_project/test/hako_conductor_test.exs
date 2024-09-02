defmodule HakoConductorTest do
  use ExUnit.Case
  doctest HakoConductor

  test "initializes the conductor" do
    # conductor_start/2 の正しい引数を指定してください
    delta_usec = 10000
    max_delay_usec = 20000

    assert HakoConductor.start(delta_usec, max_delay_usec) == :ok
    assert HakoConductor.stop() == :ok
  end
end
