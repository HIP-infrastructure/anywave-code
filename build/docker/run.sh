#!/bin/bash
image=$1
export UID=$(id -u)
export GID=$(id -g)

# detect matlab
if [ -d /usr/local/MATLAB ]; then
   export MATLAB="-v /usr/local/MATLAB:/usr/local/MATLAB"
fi 

docker run -it -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -v /mnt:/mnt -v /home:/home  \
     --user $UID:$GID --workdir="/home/$USER" --volume="/etc/group:/etc/group:ro" \
     --volume="/etc/passwd:/etc/passwd:ro" --volume="/etc/shadow:/etc/shadow:ro" \
     $MATLAB $image
