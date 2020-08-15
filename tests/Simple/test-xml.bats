setup() {
    run ./build/duplo -xml tests/Simple/LineNumbers.lst out.xml
}

@test "LineNumbers.c" {
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Loading and hashing files ... 2 done." ]
    [ "${lines[1]}" = "tests/Simple/LineNumbers.c found: 1 block(s)" ]
}

@test "LineNumbers.c out.xml" {
    run cat out.xml
    printf 'Lines:\n'
    printf 'lines %s\n' "${lines[@]}" >&2
    [ "${lines[0]}" = "<?xml version=\"1.0\"?>" ]
    [ "${lines[1]}" = "<duplo>" ]
    [ "${lines[2]}" = "    <set LineCount=\"5\">" ]
    [ "${lines[3]}" = "        <block SourceFile=\"tests/Simple/LineNumbers.c\" StartLineNumber=\"7\" EndLineNumber=\"13\"/>" ]
    [ "${lines[4]}" = "        <block SourceFile=\"tests/Simple/LineNumbers.c\" StartLineNumber=\"1\" EndLineNumber=\"7\"/>" ]
    [ "${lines[5]}" = "        <lines xml:space=\"preserve\">" ]
    [ "${lines[6]}" = "            <line Text=\"AAAAA\"/>" ]
    [ "${lines[7]}" = "            <line Text=\"BBBBB\"/>" ]
    [ "${lines[8]}" = "            <line Text=\"CCCCC\"/>" ]
    [ "${lines[9]}" = "            <line Text=\"DDDDD\"/>" ]
    [ "${lines[10]}" = "            <line Text=\"EEEEE\"/>" ]
    [ "${lines[11]}" = "        </lines>" ]
    [ "${lines[12]}" = "    </set>" ]
    [ "${lines[13]}" = "</duplo>" ]
}
