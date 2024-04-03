@test "simple_log.adb" {
    run ./build/duplo -ml 2 tests/Simple_Ada/simple_log.lst out.txt
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Loading and hashing files ... 2 done." ]
    [ "${lines[1]}" = "tests/Simple_Ada/simple_log.adb found: 6 block(s)" ]
}

@test "simple_log.adb out.txt" {
    run ./build/duplo -ml 2 tests/Simple_Ada/simple_log.lst out.txt
    run cat out.txt
    printf 'Lines:\n'
    printf 'lines %s\n' "${lines[@]}" >&2
    printf 'output %s\n' "${output[@]}" >&2
    [ "${lines[0]}" = "tests/Simple_Ada/simple_log.adb(29)" ]
    [ "${lines[1]}" = "tests/Simple_Ada/simple_log.adb(20)" ]
    [ "${lines[4]}" = "tests/Simple_Ada/simple_log.adb(37)" ]
    [ "${lines[5]}" = "tests/Simple_Ada/simple_log.adb(29)" ]
    [ "${lines[8]}" = "tests/Simple_Ada/simple_log.adb(45)" ]
    [ "${lines[9]}" = "tests/Simple_Ada/simple_log.adb(37)" ]
    [ "${lines[12]}" = "tests/Simple_Ada/simple_log.adb(37)" ]
    [ "${lines[13]}" = "tests/Simple_Ada/simple_log.adb(20)" ]
    [ "${lines[16]}" = "tests/Simple_Ada/simple_log.adb(45)" ]
    [ "${lines[17]}" = "tests/Simple_Ada/simple_log.adb(29)" ]
    [ "${lines[20]}" = "tests/Simple_Ada/simple_log.adb(45)" ]
    [ "${lines[21]}" = "tests/Simple_Ada/simple_log.adb(20)" ]
    [ "${lines[24]}" = "Configuration:" ]
    [ "${lines[25]}" = "  Number of files: 1" ]
    [ "${lines[26]}" = "  Minimal block size: 2" ]
    [ "${lines[27]}" = "  Minimal characters in line: 3" ]
    [ "${lines[28]}" = "  Ignore preprocessor directives: 0" ]
    [ "${lines[29]}" = "  Ignore same filenames: 0" ]
    [ "${lines[30]}" = "Results:" ]
    [ "${lines[31]}" = "  Lines of code: 33" ]
    [ "${lines[32]}" = "  Duplicate lines of code: 12" ]
    [ "${lines[33]}" = "  Total 6 duplicate block(s) found." ]
}
