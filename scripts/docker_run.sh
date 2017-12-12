#!/bin/bash

docker_build_dir="build_ubuntu_docker"

docker run -ti -u `id -u $USER` -e CC -e CXX --volume=$PWD:/home/ubuntu/shared bartekt/ubuntu-clang-cmake:latest\
    /bin/bash -c 'mkdir -p /tmp/build/ && cd /tmp/build && cmake /home/ubuntu/shared -DCMAKE_BUILD_TYPE=Debug -DGG_BUILD_TESTS=ON && make -j$(nproc) && make test'
