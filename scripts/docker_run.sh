#!/bin/bash

docker_build_dir="build_ubuntu_docker"

docker run -ti -u `id -u $USER` $ci_env -e CC -e CXX -e GG_BUILD_TESTS --volume=$PWD:/home/ubuntu/shared bartekt/ubuntu-clang-cmake:latest\
    /bin/bash -c 'mkdir -p /tmp/build/ && cd /tmp/build && cmake /home/ubuntu/shared -DCMAKE_BUILD_TYPE=Debug -DGG_BUILD_TESTS=${GG_BUILD_TESTS} && make -j$(nproc) && make test && bash <(curl -s https://codecov.io/bash) || echo "Codecov did not collect coverage reports"'
