#!/bin/bash
# first arg must be version number
cp -r /usr/local/AnyWave AnyWave
docker build . -t anywave:$1
