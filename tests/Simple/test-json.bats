#!/bin/bash

@test "LineNumbers.c" {
    run ./build/duplo -json tests/Simple/LineNumbers.lst out.json
    [ "$status" -eq 1 ]
    [ "${lines[0]}" = "Loading and hashing files ... 2 done." ]
}

@test "LineNumbers.c out.json" {
    run diff <(cat tests/Simple/expected-json.log) <(./build/duplo -json tests/Simple/LineNumbers.lst -)
    [ "$status" -eq 0 ]
    printf 'Lines:\n'
    printf 'lines %s\n' "${lines[@]}" >&2
    printf 'output %s\n' "${output[@]}" >&2
}
