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

