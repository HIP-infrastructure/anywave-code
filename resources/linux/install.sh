#!/bin/bash
# install function
install() {
    if [ ! -d $1 ]; then
        mkdir $1
    else 
        rm -r $1
        mkdir $1
    fi
    chmod 755 anywave.sh
    cp anywave.sh $1
    cp AnyWaveLinux $1
    cp -r lib $1
    cp -r Plugins $1
    cp -r Python $1  
    cp -r include $1
    cp -r armadillo $1
}


if [ $# = 0 ]
then
    echo "AnyWave will be installed on /usr/local/AnyWave by default."
    echo "root privileges may be required to install AnyWave."
    echo "You may specify a custom path for installation using ./install.sh PATH"
    INSTALL_DIR=/usr/local/AnyWave
    CUSTOM=0
else
    INSTALL_DIR=$1
    CUSTOM=1
fi
echo
read -p "Proceed with installation (y/n)? " -n 1
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
   echo "Installing..."
   install $INSTALL_DIR $CUSTOM
   if [[ $CUSTOM = 1 ]]; then
       ln -s $INSTALL_DIR/anywave.sh $INSTALL_DIR/anywave 
       echo "Add the specified path to your $PATH variable and type: anywave"
   else
       rm /usr/local/bin/anywave
       ln -s $INSTALL_DIR/anywave.sh /usr/local/bin/anywave 
       echo "Launch AnyWave using 'anywave' in a terminal."
   fi
else
    exit
fi

