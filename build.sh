#!/bin/bash

name=libxstring
src=src/*.c
cc=gcc
inc=-I./

flags=(
    -std=c99
    -Wall
    -O2
)

mac_dlib() {
    $cc ${flags[*]} $inc -mmacos-version-min=10.10 -dynamiclib $src -o $name.dylib
}

linux_dlib() {
    $cc -shared ${flags[*]} $inc -fPIC $src -o $name.so
}

dlib() {
    if echo "$OSTYPE" | grep -q "darwin"; then
        mac_dlib
    elif echo "$OSTYPE" | grep -q "linux"; then
        linux_dlib
    else
        echo "OS not supported yet..." && exit
    fi
}

slib() {
    $cc ${flags[*]} $inc -c $src && ar -crv $name.a *.o && rm *.o
}

cleanf() {
    [ -f $1 ] && rm $1 && echo "Deleted '$1'."
}

clean() {
    cleanf $name.a
    cleanf $name.so
    cleanf $name.dylib
}

case "$1" in
    "static")
        slib;;
    "dinamic")
        dlib;;
    "clean")
        clean;;
    *)
        echo "Use with 'static' or 'dinamic' for compilation.";;
esac
