#!/bin/sh

set -ue
input=$1
output=$2

exec > "$output"
echo '# Project 2 Testing Check-In'
echo '![](screenshot.png)'
echo '```cpp'
cat "$input"
echo '```'
