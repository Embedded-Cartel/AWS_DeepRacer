cd AUTOSAR/bermuda/gen_ara/implementation/Machine 
cmake -B build -D CMAKE_INSTALL_PREFIX=$PARA_SDK
cmake --build build -j $(nproc)
cmake --install build
rm -rf build
