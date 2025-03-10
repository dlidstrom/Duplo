#!/bin/bash

@test "LineNumbers.c" {
    run ./build/duplo tests/Simple/LineNumbers.lst out.txt
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Loading and hashing files ... 2 done." ]
    [ "${lines[1]}" = "tests/Simple/LineNumbers.c found: 1 block(s)" ]
}

@test "LineNumbers.c out.txt" {
    run diff <(cat tests/Simple/expected.log) <(./build/duplo tests/Simple/LineNumbers.lst -)
    [ "$status" -eq 0 ]
    printf 'Lines:\n'
    printf 'lines %s\n' "${lines[@]}" >&2
    printf 'output %s\n' "${output[@]}" >&2
}
