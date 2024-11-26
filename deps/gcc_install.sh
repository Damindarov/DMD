#!/bin/bash

GPP_VERSION=$(g++ --version | grep g++ | awk '{print $NF}' | awk -F '.' '{print $1}')

if [ ${GPP_VERSION} -ge 10 ]; then
	echo "Suitable g++ version already installed"
	exit 0
else
	sudo apt-get install g++-10 gcc-10 -y
	sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 20
	sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 20
	sudo update-alternatives --install /usr/bin/cc cc /usr/bin/gcc 30
	sudo update-alternatives --set cc /usr/bin/gcc
	sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++ 30
	sudo update-alternatives --set c++ /usr/bin/g++

	sudo update-alternatives --set gcc "/usr/bin/gcc-10"
	sudo update-alternatives --set gcc "/usr/bin/g++-10"
	exit 0
fi
