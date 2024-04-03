@test "g_chase.c" {
    run ./build/duplo tests/Quake2/files.lst out.txt
    run cat out.txt
    printf 'lines %s\n' "${lines[@]}" >&2
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Loading and hashing files ... 2 done." ]
    [ "${lines[1]}" = "tests/Quake2/g_chase.c found: 1 block(s)" ]
}

@test "simple" {
}
