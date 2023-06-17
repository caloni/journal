rem @echo off
pushd src
if not exist build mkdir build
cd build
cmake ..
cmake --build .
move Debug\*.exe ..\..\scripts
popd
