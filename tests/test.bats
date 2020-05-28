@test "invoking without arguments will show help text" {
    run ./build/duplo
    printf 'lines %s\n' "${lines[@]}" >&2
    echo "status = $status"
    [ "$status" -eq 1 ]
    [ "${lines[0]}" = "NAME" ]
    [ "${lines[1]}" = "       Duplo $DUPLO_VERSION - duplicate source code block finder" ]
    [ "${lines[2]}" = "SYNOPSIS" ]
    [ "${lines[3]}" = "       duplo [OPTIONS] [INTPUT_FILELIST] [OUTPUT_FILE]" ]
    [ "${lines[4]}" = "DESCRIPTION" ]
    [ "${lines[5]}" = "       Duplo is a tool to find duplicated code blocks in large" ]
    [ "${lines[6]}" = "       C/C++/Java/C#/VB.Net software systems." ]
    [ "${lines[7]}" = "       -ml              minimal block size in lines (default is 4)" ]
    [ "${lines[8]}" = "       -pt              percentage of lines of duplication threshold to override -ml" ]
    [ "${lines[9]}" = "                        (default is 100%)" ]
    [ "${lines[10]}" = "                        useful for identifying whole file duplication" ]
    [ "${lines[11]}" = "       -mc              minimal characters in line (default is 3)" ]
    [ "${lines[12]}" = "                        lines with less characters are ignored" ]
    [ "${lines[13]}" = "       -ip              ignore preprocessor directives" ]
    [ "${lines[14]}" = "       -d               ignore file pairs with same name" ]
    [ "${lines[15]}" = "       -xml             output file in XML" ]
    [ "${lines[16]}" = "       INPUT_FILELIST   input filelist (specify '-' to read from stdin)" ]
    [ "${lines[17]}" = "       OUTPUT_FILE      output file" ]
    [ "${lines[18]}" = "VERSION" ]
    [ "${lines[19]}" = "       $DUPLO_VERSION" ]
    [ "${lines[20]}" = "AUTHORS" ]
    [ "${lines[21]}" = "       Daniel Lidstrom (dlidstrom@gmail.com)" ]
    [ "${lines[22]}" = "       Christian M. Ammann (cammann@giants.ch)" ]
    [ "${lines[23]}" = "       Trevor D'Arcy-Evans (tdarcyevans@hotmail.com)" ]
}

@test "simple" {
}
