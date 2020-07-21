#!/bin/bash
# install function
install() {
    if [ ! -d $1 ]; then
        mkdir $1
    else 
        rm -r $1
        mkdir $1
    fi
#    chmod 755 anywave.sh
#    cp anywave.sh $1
    cp AnyWaveLinux $1
    cp -r lib $1
    cp -r Plugins $1
    cp -r plugins $1
    cp -r qmake $1
    cp -r Python $1  
    cp -r include $1
    cp -r armadillo $1
}


echo "AnyWave will be installed on /usr/local/AnyWave by default."
echo "root privileges may be required to install to that location."
INSTALL_DIR=/usr/local/AnyWave
BIN_DIR=/usr/local/bin

echo
read -p "Proceed with installation (y/n)? " -n 1
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
   echo "Installing..."
   install $INSTALL_DIR 
   if [ -f /usr/local/bin/anywave ]; then
     rm /usr/local/bin/anywave
   fi
   ln -s $INSTALL_DIR/AnyWaveLinux /usr/local/bin/anywave 
   echo "Done. Launch AnyWave using 'anywave' in a terminal."
else
    exit
fi

