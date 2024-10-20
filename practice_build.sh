cd /home/ubuntu/AWS_DeepRacer/AUTOSAR/EC2024_PRACTICE_VER1/gen_ara/implementation/Machine 
cmake -B build -D CMAKE_INSTALL_PREFIX=$PARA_SDK
cmake --build build -j $(nproc)
cmake --install build
