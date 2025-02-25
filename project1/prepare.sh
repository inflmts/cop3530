#!/bin/sh

set -ue

rm -rf dist
mkdir dist
cp avl.cpp avl.h shell.cpp shell.h main.cpp dist
sed 's|^|//|' test.cpp > dist/test.cpp
cp report/report.pdf dist/Report.pdf
