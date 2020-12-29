sudo apt-get install zlib1g-dev openssl libssl-dev libcurl4-openssl-dev uuid-dev default-libmysqlclient-dev build-essential make
apt-cache search libevent
sudo apt install libevent-dev
sudo apt install cmake

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