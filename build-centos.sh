yum -y install zlib-devel openssl-devel curl-devel libuuid-devel mysql-devel libevent-devel gcc make cmake

cd deps
cd hiredis
cmake .
make
make install
cd ..
cd ..
cmake .
make
make install
cd lib
cmake .
make
cd ..