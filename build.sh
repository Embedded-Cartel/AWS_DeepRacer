#!/bin/bash

cd AA_Code 

  find ./EM ./CM -type f -name "*.json" -exec sed -i 's/192.168.20.27/127.0.0.1/g' {} \;
  find ./EM ./CM -type f -name "*.json" -exec sed -i 's/192.168.29.253/127.0.0.1/g' {} \;
  find ./EM ./CM -type f -name "*.json" -exec sed -i 's/172.31.16.137/127.0.0.1/g' {} \;
  find ./EM ./CM -type f -name "*.json" -exec sed -i 's/wlo1/lo/g' {} \;
  find ./EM ./CM -type f -name "*.json" -exec sed -i 's/ens5/lo/g' {} \;
  find ./EM ./CM -type f -name "*.json" -exec sed -i 's/mlan0/lo/g' {} \;

cmake -B build -D CMAKE_INSTALL_PREFIX=$PARA_SDK
cmake --build build -j $(nproc)
cmake --install build
rm -rf build
