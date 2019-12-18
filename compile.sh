#!/bin/bash
p() {
    now="$(date +'%r')"
    printf "$(tput setaf 1)%s$(tput sgr0) | $(tput bold)$1$(tput sgr0)\n" "$now";
}

p "compiling..." &&
make &&

p "OK!" &&
p "waiting for new changes...";
