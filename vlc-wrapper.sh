#!/bin/bash
export XDG_RUNTIME_DIR=/run/user/1000
exec mpv "$@" 
# mpv "$@"