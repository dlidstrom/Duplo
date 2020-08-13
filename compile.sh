#!/bin/bash
# to run this, first set the DUPLO_VERSION environment variable. Otherwise
# some of the tests might fail. So, do this:
# > export DUPLO_VERSION=v1.0.0
p() {
    now="$(date +'%r')"
    printf "$(tput setaf 1)%s$(tput sgr0) | $(tput bold)$1$(tput sgr0)\n" "$now";
}

err() {
    echo "Error: $*" >> /dev/stderr
}

if [ ! -d "build" ]; then
    err "You need to initialize first."
    err "Create a \"build\" folder and issue \"cmake ..\" in there."
    exit
fi

p "compiling..." &&
pushd build
make &&
popd &&
bats --recursive tests &&
p "OK!";
