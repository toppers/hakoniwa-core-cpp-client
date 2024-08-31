defmodule ElixirHelloWorldTest do
  use ExUnit.Case
  doctest ElixirHelloWorld

  test "greets the world" do
    assert ElixirHelloWorld.hello() == :world
  end
end
