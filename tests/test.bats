#!/bin/bash

@test "invoking without arguments will show help text" {
    run ./build/duplo
    printf 'lines %s\n' "${lines[@]}" >&2
    echo "status = $status"
    [ "$status" -eq 1 ]
    [ "${lines[0]}" = "NAME" ]
    [ "${lines[2]}" = "SYNOPSIS" ]
    [ "${lines[3]}" = "       duplo [OPTIONS] [INPUT_FILELIST] [OUTPUT_FILE]" ]
    [ "${lines[4]}" = "DESCRIPTION" ]
    [ "${lines[5]}" = "       Duplo is a tool to find duplicated code blocks in large" ]
    [ "${lines[6]}" = "       C/C++/Java/C#/VB.Net/Ada software systems." ]
    [ "${lines[7]}" = "       -ml              minimal block size in lines (default is 4)" ]
    [ "${lines[8]}" = "       -pt              percentage of lines of duplication threshold to override -ml" ]
    [ "${lines[9]}" = "                        (default is 100%)" ]
    [ "${lines[10]}" = "                        useful for identifying whole file duplication" ]
    [ "${lines[11]}" = "       -mc              minimal characters in line (default is 3)" ]
    [ "${lines[12]}" = "                        lines with less characters are ignored" ]
    [ "${lines[13]}" = "       -n               only report for first N files" ]
    [ "${lines[14]}" = "       -j               number of threads to use (default is 1)" ]
    [ "${lines[15]}" = "       -ip              ignore preprocessor directives" ]
    [ "${lines[16]}" = "       -d               ignore file pairs with same name" ]
    [ "${lines[17]}" = "       -xml             output file in XML" ]
    [ "${lines[18]}" = "       -json            output file in JSON format" ]
    [ "${lines[19]}" = "       INPUT_FILELIST   input filelist (specify '-' to read from stdin)" ]
    [ "${lines[20]}" = "       OUTPUT_FILE      output file (specify '-' to output to stdout)" ]
    [ "${lines[21]}" = "VERSION" ]
    [ "${lines[23]}" = "AUTHORS" ]
    [ "${lines[24]}" = "       Daniel Lidstrom (dlidstrom@gmail.com)" ]
    [ "${lines[25]}" = "       Christian M. Ammann (cammann@giants.ch)" ]
    [ "${lines[26]}" = "       Trevor D'Arcy-Evans (tdarcyevans@hotmail.com)" ]
    [ "${lines[27]}" = "       Christos Gkantidis (cgkantid@proton.me)" ]
}
