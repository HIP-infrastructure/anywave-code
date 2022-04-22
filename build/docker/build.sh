#!/bin/bash
docker build ubuntu-aw -t ubuntu-aw:latest < ubuntu-aw/Dockerfile
# first arg must be version number
docker build /usr/local/ -t anywave:latest -f Dockerfile
# build MCR version
docker build  /usr/local/MATLAB -t anywave-mcr:latest  < anywave-mcr/Dockerfile
