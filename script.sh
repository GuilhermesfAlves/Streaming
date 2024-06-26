#!/bin/bash

if [ "$1" == "client" ]; then
    sudo ./exec client 8080
elif [ "$1" == "server" ]; then
    sudo ./exec server 8080
else
    echo "Usage: $0 { client | server }"
    exit 1
fi