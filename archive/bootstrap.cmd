@echo off
pushd src
if not exist build mkdir build
cd build
cmake ..
cmake --build . --config RelWithDebInfo
cmake --install . --config RelWithDebInfo
popd
