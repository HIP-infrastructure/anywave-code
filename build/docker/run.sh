#!/bin/bash
export UID=$(id -u)
export GID=$(id -g)

docker run -it -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -v /mnt:/mnt -v /home:/home  \
     --user $UID:$GID --workdir="/home/$USER" --volume="/etc/group:/etc/group:ro" \
     --volume="/etc/passwd:/etc/passwd:ro" --volume="/etc/shadow:/etc/shadow:ro" \
     -v /usr/local/MATLAB:/usr/local/MATLAB anywave:2.3.2
