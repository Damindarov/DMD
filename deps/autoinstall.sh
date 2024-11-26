#!/bin/bash

sudo apt update

UBUNTU_VERSION=$(lsb_release -r | awk '{print $2}')

if [ ${UBUNTU_VERSION}=="20.04" ]; then
        echo "Your ubuntu is 20 ubuntu"
	SUFFIX="ubuntu20"
elif [ ${UBUNTU_VERSION}=="22.04" ]; then
        echo "Your ubuntu is 22 ubuntu"
	SUFFIX="ubuntu22"
else
        echo "Unknown system"
	exit 1
fi

rm -rf ${SUFFIX}_package && mkdir ${SUFFIX}_package && cd ${SUFFIX}_package

curl -s https://api.github.com/repos/MeshInspector/MeshLib/releases/latest \
| grep "browser_download_url\".*${SUFFIX}-dev.deb" \
| cut -d : -f 2,3 \
| tr -d \" \
| wget -qi -

sudo apt install ./*.deb -y

cd ../test/simple_application/
rm -rf build
mkdir build && cd build
sudo cmake -DCMAKE_CXX_COMPILER=g++-10 ..
sudo cmake --build .
sudo cmake --install .
./test
