#!/usr/bin/env sh
set -e
BASEDIR=$(dirname "$0")
cd "$BASEDIR"

rm -f experiment5.zip
zip -r experiment5.zip Experiment5/
