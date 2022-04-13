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

dlib() {
    if echo "$OSTYPE" | grep -q "darwin"; then
        $cc ${flags[*]} $inc -mmacos-version-min=10.10 -dynamiclib $src -o $name.dylib
    elif echo "$OSTYPE" | grep -q "linux"; then
        $cc -shared ${flags[*]} $inc -fPIC $src -o $name.so
    else
        echo "OS not supported yet..." && exit
    fi
}

slib() {
    $cc ${flags[*]} $inc -c $src && ar -cr $name.a *.o && rm *.o
}

install() {
    [ "$EUID" -ne 0 ] && echo "Run with to sudo to install in /usr/local" && exit

    slib && dlib
    cp xstring.h /usr/local/include/

    [ -f $name.a ] && mv $name.a /usr/local/lib
    [ -f $name.so ] && mv $name.so /usr/local/lib
    [ -f $name.dylib ] && mv $name.dylib /usr/local/lib
    return 0
}

cleanf() {
    [ -f $1 ] && rm $1 && echo "Deleted '$1'."
}

clean() {
    cleanf $name.a
    cleanf $name.so
    cleanf $name.dylib
    return 0
}

case "$1" in
    "static")
        slib;;
    "dinamic")
        dlib;;
    "clean")
        clean;;
    "install")
        install;;
    *)
        echo "Use with 'static' or 'dinamic' for compilation.";;
esac
