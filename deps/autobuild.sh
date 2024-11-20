sudo apt update
sudo apt install cmake libgdcm-tools libboost-all-dev libvtkgdcm-cil libvtkgdcm-dev libvtkgdcm-java libvtkgdcm-tools python3-vtkgdcm zlib1g-dev

# git clone https://github.com/emscripten-core/emsdk.git
# cd emsdk
# ./emsdk install 3.1.48
# ./emsdk activate 3.1.48
# source ./emsdk_env.sh

git config --global http.postBuffer 524288000
git clone --recurse-submodules https://github.com/MeshInspector/MeshLib.git && cd MeshLib

echo "N" | ./scripts/build_thirdparty.sh
echo "N y y" | ./scripts/build_source.sh
./scripts/distribution.sh

sudo rm /usr/local/lib/MeshLib/lib/libopenvdb.so.10.1 && sudo ln -s /usr/local/lib/MeshLib/lib/libopenvdb.so.10.1.0 /usr/local/lib/MeshLib/lib/libopenvdb.so.10.1
sudo rm /usr/local/lib/MeshLib/lib/libE57Format.so.3 && sudo ln -s /usr/local/lib/MeshLib/lib/libE57Format.so.3.2.0 /usr/local/lib/MeshLib/lib/libE57Format.so.3
sudo rm /usr/local/lib/MeshLib/lib/libcpr.so.1 && sudo ln -s /usr/local/lib/MeshLib/lib/libcpr.so.1.8.1 /usr/local/lib/MeshLib/lib/libcpr.so.1
sudo rm /usr/local/lib/MeshLib/lib/libz.so.1 && sudo ln -s /usr/local/lib/MeshLib/lib/libz.so.1.2.11.zlib-ng /usr/local/lib/MeshLib/lib/libz.so.1

sudo apt install ./distr/meshlib-dev.deb

echo "Package installed, next: checkup"

cd examples/cpp-examples/
mkdir build
cd build
cmake ..
cmake --build .
./MRBasedMainFunc
