#!/bin/sh
#  make_dmg.sh
#
#  Created by bruno colombet on 22/03/2016.
#
# launch the script with sudo


# do the install name process before
./install_names.sh ../../qmake_build/release/bin/AnyWave.app/Contents/Frameworks

hdiutil create -volname AnyWave -srcfolder build/AnyWave.mpkg -ov -format UDZO AnyWave.dmg
hdiutil create -volname AnyWaveINS -srcfolder build/AnyWaveINS.mpkg -ov -format UDZO AnyWaveINS.dmg

