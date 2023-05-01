#!/usr/bin/sh

GREEN="\e[32m"
RESET="\e[0m"
RUN="$GREEN[RUN]$RESET"

# https://earthly.dev/blog/creating-and-hosting-your-own-deb-packages-and-apt-repo/
echo "$RUN Cleaning..."
rm -rf build/
echo "$RUN Building..."
./build
echo "$RUN Structuring..."
mkdir -p build/DEBIAN
mkdir -p build/usr/local/bin
mv build/out build/usr/local/bin/ns4l
cp deb build/DEBIAN/control
echo "$RUN Building package..."
dpkg --build build/
