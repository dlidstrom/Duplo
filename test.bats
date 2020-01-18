@test "invoking without arguments will show help text" {
    run ./build/duplo
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "NAME" ]
    [ "${lines[1]}" = "       Duplo 0.4.0 - duplicate source code block finder" ]
}
