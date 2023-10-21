#!/usr/bin/env sh
set -e
BASEDIR=$(dirname "$0")
cd "$BASEDIR"

rm -f experiment3.zip
zip -r experiment3.zip Experiment3/
