#!/bin/bash

docker run -ti -u `id -u $USER` --volume=$PWD:/home/ubuntu/shared ubuntu-clang:latest\
    /bin/bash -c 'cd /home/ubuntu/shared && mkdir -p build_ubuntu_docker && cd build_ubuntu_docker && cmake .. -DCMAKE_BUILD_TYPE=Debug -DGG_BUILD_TESTS=ON && make -j$(nproc)'
