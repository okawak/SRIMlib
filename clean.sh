#!/usr/bin/zsh

if [ -e "./build/install_manifest.txt" ] ; then
    xargs sudo rm -rf < ./build/install_manifest.txt
fi

rm -rf install
rm -rf build
rm -rf thisSRIMlib.sh
rm -rf db/SRIMdata.root

FIGFILES=`find ./figure/SRIM/ -maxdepth 1 -name "*.png" 2>/dev/null`
if [ $? -ne 0 ]; then
    echo "ERROR"
elif [ -n "$FIGFILES" ]; then
    rm -rf ./figure/SRIM/*.png
fi
