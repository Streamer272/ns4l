#!/usr/bin/sh

GREEN="\e[32m"
RESET="\e[0m"
RUN="$GREEN[RUN]$RESET"

echo "$RUN Building..."
./build.sh
echo "$RUN Running..."
./build/out
echo "$RUN Finished"
