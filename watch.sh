#!/bin/bash

p() {
    now="$(date +'%r')"
    printf "$(tput setaf 1)%s$(tput sgr0) | $(tput bold)$1$(tput sgr0)\n" "$now";
}

./compile.sh &

fswatch \
    --extended \
    --latency 0.5 \
    --exclude ".*" \
    --include "Makefile|(\.(h|cpp|bats|c)$)" . |
while read line; do
    p "new changes received: $line"

    # Find the PIDs
    pids=$(ps -ef | grep "compile.sh" | grep -v grep | awk '{print $2}')

    # Kill them
    if [ -n "$pids" ]; then
        p "cancelling actual event..."; kill -9 $pids &> /dev/null
    fi

    ./compile.sh &
done
