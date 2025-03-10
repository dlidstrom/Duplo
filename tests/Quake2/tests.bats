#!/bin/bash

@test "g_chase.c" {
    run diff <(cat tests/Quake2/expected.log) <(./build/duplo tests/Quake2/files.lst -)
    [ "$status" -eq 0 ]
    printf 'Lines:\n'
    printf 'lines %s\n' "${lines[@]}" >&2
    printf 'output %s\n' "${output[@]}" >&2
}
