#!/bin/bash

cd AA_Sample_Code

if ifconfig | grep -q "172.31.16.137"; then
  echo "EC2"

  find ./EM ./CM -type f -name "*.json" -exec sed -i 's/192.168.20.27/172.31.16.137/g' {} \;
  find ./EM ./CM -type f -name "*.json" -exec sed -i 's/wlo1/ens5/g' {} \;

elif ifconfig | grep -q "192.168.0.10"; then
  echo "DeepRacer"

  find ./EM ./CM -type f -name "*.json" -exec sed -i 's/192.168.20.27/192.168.0.10/g' {} \;
  find ./EM ./CM -type f -name "*.json" -exec sed -i 's/wlo1/mlan0/g' {} \;

elif ifconfig | grep -q "192.168.29.253"; then
  echo "DeepRacer"

  find ./EM ./CM -type f -name "*.json" -exec sed -i 's/192.168.20.27/192.168.29.253/g' {} \;
  find ./EM ./CM -type f -name "*.json" -exec sed -i 's/wlo1/mlan0/g' {} \;

else
  echo "IP not found"=
  find ./EM ./CM -type f -name "*.json" -exec sed -i 's/192.168.20.27/127.0.0.1/g' {} \;
  find ./EM ./CM -type f -name "*.json" -exec sed -i 's/wlo1/lo/g' {} \;
fi

cmake -B build -D CMAKE_INSTALL_PREFIX=$PARA_SDK
cmake --build build -j $(nproc)
cmake --install build
rm -rf build
