#!/bin/bash

user_str="$1"
str_len=${#user_str}
idx=/sys/module/my_module/parameters/idx
ch_val=/sys/module/my_module/parameters/ch_val

function set_idx()
{
    echo "$1" > $idx
}

function set_ch_val()
{
    echo "$1" > $ch_val
}

for ((i=0; i<str_len; i++)); do
    set_idx "$i"
    char="${user_str:$i:1}"
    ascii_val=$(printf "%d" "'$char'")

    set_ch_val "$ascii_val"

    echo "Sent: idx=$idx, char=$ascii_val"
done
