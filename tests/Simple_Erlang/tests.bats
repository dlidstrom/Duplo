#!/bin/bash

@test "simple_logger.erl" {
    run ./build/duplo -ml 2 tests/Simple_Erlang/simple_logger.lst out.txt
    [ "$status" -eq 1 ]
    [ "${lines[0]}" = "Loading and hashing files ... 2 done." ]
    [ "${lines[1]}" = "tests/Simple_Erlang/simple_logger.erl found: 3 block(s)" ]
}

@test "simple_logger.erl out.txt" {
    run ./build/duplo -ml 2 tests/Simple_Erlang/simple_logger.lst -
    [ "$status" -eq 1 ]
    run diff <(cat tests/Simple_Erlang/expected.log) <(./build/duplo -ml 2 tests/Simple_Erlang/simple_logger.lst -)
    [ "$status" -eq 0 ]
    printf 'Lines:\n'
    printf 'lines %s\n' "${lines[@]}" >&2
    printf 'output %s\n' "${output[@]}" >&2
}
