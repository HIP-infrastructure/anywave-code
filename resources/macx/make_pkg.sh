 #!/bin/bash

APP_DIR=/Users/bruno/dev/anywave-mac/build/AnyWave.app
ID="Apple Development: thechewie13@gmail.com (MY3J8AN9H6)"
source ./sign.sh

mv ./AnyWave.app/Contents/Plugins/libDeltamedReader.dylib .

sudo pkgbuild --install-location /Applications --identifier fr.univ-amu.ins.anywave --version 1 \
  --scripts ./scripts --component ./AnyWave.app ./AnyWave.pkg

sudo codesign -s "$ID" ./AnyWave.pkg

sudo hdiutil create -volname AnyWave -srcfolder ./AnyWave.pkg -ov -format UDZO ./AnyWave.dmg
sudo codesign -s "$ID" ./AnyWave.dmg

mv ./libDeltamedReader.dylib ./AnyWave.app/Contents/Plugins

sudo pkgbuild --install-location /Applications --identifier fr.univ-amu.ins.anywave --version 1 \
  --scripts ./scripts --component ./AnyWave.app ./AnyWaveINS-$1.pkg

sudo hdiutil create -volname AnyWave -srcfolder ./AnyWaveINS-$1.pkg -ov -format UDZO ./AnyWaveINS-$1.dmg
sudo codesign -s "$ID" ./AnyWaveINS-$1.dmg
