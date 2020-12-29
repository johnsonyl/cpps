sudo yum -y install zlib-devel openssl-devel curl-devel libuuid-devel mysql-devel libevent-devel gcc gcc-c++ make cmake

cd deps
cd hiredis
cmake .
make
sudo make install
cd ..
cd ..
cmake .
make
sudo make install
cd libs
cmake .
make
cd ..