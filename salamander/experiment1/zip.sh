#!/usr/bin/env sh
set -e
BASEDIR=$(dirname "$0")
cd "$BASEDIR"

rm -f experiment.zip
zip -r experiment.zip Experiment1/
