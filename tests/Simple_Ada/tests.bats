#!/bin/bash

@test "simple_log.adb" {
    run ./build/duplo -ml 2 tests/Simple_Ada/simple_log.lst out.txt
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Loading and hashing files ... 2 done." ]
    [ "${lines[1]}" = "tests/Simple_Ada/simple_log.adb found: 6 block(s)" ]
}

@test "simple_log.adb out.txt" {
    ./build/duplo -ml 2 tests/Simple_Ada/simple_log.lst -
    run diff <(cat tests/Simple_Ada/expected.log) <(./build/duplo -ml 2 tests/Simple_Ada/simple_log.lst -)
    [ "$status" -eq 0 ]
    printf 'Lines:\n'
    printf 'lines %s\n' "${lines[@]}" >&2
    printf 'output %s\n' "${output[@]}" >&2
}
