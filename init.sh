#!/bin/sh

LIB_CONFIG=libconfig-1.7.2

# init - initiaize orcd

# download libconfig header
curl -sl https://raw.githubusercontent.com/hyperrealm/libconfig/master/lib/libconfig.h++ -o ./include/libconfig.hpp

# make target dir
mkdir -p target

# download and build libconfig
curl -sl https://hyperrealm.github.io/libconfig/dist/$LIB_CONFIG.tar.gz -o ./target/$LIB_CONFIG.tar.gz

cd target
tar -xzf $LIB_CONFIG.tar.gz

cd $LIB_CONFIG

./configure

make install
