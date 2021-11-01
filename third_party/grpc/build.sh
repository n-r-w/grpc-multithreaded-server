#!/bin/bash

# сборка GRPC и установка его в prebuilds/grpc/

GRPC_VERSION="1.41.1"

CPU_COUNT=$([ $(uname) = 'Darwin' ] && 
                       sysctl -n hw.logicalcpu_max || 
                       lscpu -p | egrep -v '^#' | wc -l)
echo $CPU_COUNT

MAIN_DIR=$PWD
INSTALL_DIR_REL=./../../prebuilds/grpc/
rm -rf $INSTALL_DIR_REL
mkdir -p $INSTALL_DIR_REL

INSTALL_DIR_ABS="$(dirname $(readlink -e $INSTALL_DIR_REL))/$(basename $INSTALL_DIR_REL)"
SOURCES=$PWD/sources

PATH="$INSTALL_DIR_ABS/bin:$PATH"

echo "GRPC target folder: "$INSTALL_DIR_ABS
echo "GRPC version:"$GRPC_VERSION

rm -rf $SOURCES
mkdir -p $SOURCES
cd $SOURCES
git clone --recurse-submodules -b v$GRPC_VERSION https://github.com/grpc/grpc .
cd $MAIN_DIR

rm -rf ./../../build/grpc
mkdir -p ./../../build/grpc
cd ./../../build/grpc

#cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DgRPC_SSL_PROVIDER=package -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR_ABS $SOURCES
cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR_ABS $SOURCES

make -j$CPU_COUNT
make install
