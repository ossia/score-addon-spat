#!/bin/bash
rm -rf release
mkdir -p release

cp -rf Spat *.{hpp,cpp,txt,json} LICENSE release/

mv release score-addon-spat
7z a score-addon-spat.zip score-addon-spat
