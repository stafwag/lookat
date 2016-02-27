#!/bin/sh

cppcheck --xml --xml-version=2 --enable=all --inconclusive --language=c "*.c"
