rm -rf sdk
mkdir sdk
cp para_deepracer_release.tar.gz sdk
cd sdk
tar -zxvf para_deepracer_release.tar.gz

#lidar sdk download
cd ..
git clone git@github.com:Slamtec/rplidar_sdk.git
cd ./rplidar_sdk/sdk
make

cp -rf include/ ../../sdk/include/lidar
cd ../  # rplidar_sdk/
cp output/Linux/Release/* ../sdk/lib/

#export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
export PARA_SDK=$(pwd)/sdk
export PARA_CORE=$PARA_SDK
export PARA_CONF=$PARA_SDK/etc
export PARA_APPL=$PARA_SDK/opt
export PARA_DATA=$PARA_SDK/var
export LD_LIBRARY_PATH=$PARA_SDK/lib
