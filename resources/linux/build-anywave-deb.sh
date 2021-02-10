#!/bin/bash
sudo cp -r /usr/local/AnyWave/* anywave-amd64/usr/local/AnyWave
dpkg-deb --build anywave-amd64

mv anywave-amd64.deb anywave-1.0_amd64.deb
