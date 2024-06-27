#!/bin/bash

if [ "$1" == "client" ]; then
    sudo ./exec client 12345
elif [ "$1" == "server" ]; then
    sudo ./exec server 12345
else
    echo "Usage: $0 { client | server }"
    exit 1
fi