#!/usr/bin/env sh
set -e
BASEDIR=$(dirname "$0")
cd "$BASEDIR"

rm -f experiment2.zip
zip -r experiment2.zip Experiment2/
