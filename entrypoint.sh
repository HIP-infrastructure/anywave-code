#!/bin/bash

# what X display to use
export DISPLAY=:0 

# start X VNC server
#Xvnc $DISPLAY -geometry 1920x1200 &
Xvfb :0 -screen 0 1280x1024x16 &
fluxbox &
x11vnc -forever &

# note process id of X save
xpid=$! 

# start with all arguments to this script
anywave $@ & 
awpid=$!

# wait for AnyWave to finish,
wait $awpid

# then stop the VNC server (kill -2 is Ctrl-C)
kill -2 $xpid 