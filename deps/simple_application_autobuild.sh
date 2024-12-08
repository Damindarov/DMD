cd test/simple_application/
sudo rm -rf build
mkdir build && cd build
cmake ..
cmake --build . --parallel
sudo cmake --install .
./test
