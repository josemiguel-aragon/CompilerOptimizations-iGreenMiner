#!/usr/bin/bash
cd compilationFolder
cp DBDemo Payload/DBDemo.app/
codesign --force --sign $1 --entitlements DBDemo.app.xcent Payload/DBDemo.app
zip -r payload.ipa Payload/
ideviceinstaller -n -i payload.ipa
cd ..