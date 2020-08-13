@test "LineNumbers.c" {
    run ./build/duplo tests/Simple/LineNumbers.lst out.txt && cat out.txt
    printf 'lines %s\n' "${lines[@]}" >&2
    echo "status = $status"
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Loading and hashing files ... 2 done." ]
    [ "${lines[1]}" = "tests/Simple/LineNumbers.c found: 1 block(s)" ]
    [ "${lines[1]}" = "tests/Simple/LineNumbers.c(6)" ]
}
