setup() {
    run ./build/duplo tests/Simple/LineNumbers.lst out.txt
}

@test "LineNumbers.c" {
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Loading and hashing files ... 2 done." ]
    [ "${lines[1]}" = "tests/Simple/LineNumbers.c found: 1 block(s)" ]
}

@test "LineNumbers.c out.txt" {
    run cat out.txt
    printf 'Lines:\n'
    printf 'lines %s\n' "${lines[@]}" >&2
    printf 'output %s\n' "${output[@]}" >&2
    [ "${lines[0]}" = "tests/Simple/LineNumbers.c(7)" ]
    [ "${lines[1]}" = "tests/Simple/LineNumbers.c(1)" ]
    [ "${lines[2]}" = "AAAAA" ]
    [ "${lines[3]}" = "BBBBB" ]
    [ "${lines[4]}" = "CCCCC" ]
    [ "${lines[5]}" = "DDDDD" ]
    [ "${lines[6]}" = "EEEEE" ]
    [ "${lines[7]}" = "Configuration:" ]
    [ "${lines[8]}" = "  Number of files: 1" ]
    [ "${lines[9]}" = "  Minimal block size: 4" ]
    [ "${lines[10]}" = "  Minimal characters in line: 3" ]
    [ "${lines[11]}" = "  Ignore preprocessor directives: 0" ]
    [ "${lines[12]}" = "  Ignore same filenames: 0" ]
    [ "${lines[13]}" = "Results:" ]
    [ "${lines[14]}" = "  Lines of code: 11" ]
    [ "${lines[15]}" = "  Duplicate lines of code: 5" ]
    [ "${lines[16]}" = "  Total 1 duplicate block(s) found." ]
}
