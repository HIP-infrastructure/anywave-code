#!/bin/sh
cp version.txt ../../qmake_build/release/bin/AnyWave.app/Contents/Resources/version.txt
./install_names.sh ../../qmake_build/release/bin/AnyWave.app/Contents/Frameworks
