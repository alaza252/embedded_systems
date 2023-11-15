#!/usr/bin/env sh
set -e
BASEDIR=$(dirname "$0")
cd "$BASEDIR"

rm -f experiment4.zip
zip -r experiment4.zip Experiment4/
