#!/bin/bash

docker run -ti -u `id -u $USER` --volume=$PWD:/home/ubuntu/ uilianries/docker-clang
