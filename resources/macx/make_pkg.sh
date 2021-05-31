 #!/bin/bash
mv ./AnyWave.app/Contents/Plugins/libDeltamedReader.dylib .

APP_DIR=/Users/bruno/dev/anywave-mac/build/AnyWave.app
ID="Apple Development: thechewie13@gmail.com (MY3J8AN9H6)"
source ./sign.sh

sudo pkgbuild --install-location /Applications --identifier fr.univ-amu.ins.anywave --version 1 \
  --scripts ./scripts --component ./AnyWave.app ./AnyWave.pkg

sudo codesign -s "$ID" ./AnyWave.pkg

sudo hdiutil create -volname AnyWave -srcfolder ./AnyWave.pkg -ov -format UDZO ./AnyWave.dmg
codesign -s "$ID" ./AnyWave.dmg
