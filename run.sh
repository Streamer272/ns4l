#!/usr/bin/sh

GREEN="\e[32m"
RESET="\e[0m"
RUN="$GREEN[RUN]$RESET"

echo "$RUN Compiling..."
gcc main.c -o out -lbluetooth
echo "$RUN Running..."
./out
echo "$RUN Cleaning..."
rm out
echo "$RUN Finished"
