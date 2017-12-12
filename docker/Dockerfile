FROM ubuntu:zesty 

MAINTAINER Uilian Ries <uilianries@gmail.com>

RUN apt-get update -q && apt-get install -y -q libclang-dev clang build-essential

RUN groupadd 1001 -g 1001
RUN groupadd 1000 -g 1000
RUN useradd -ms /bin/bash ubuntu -g 1001 -G 1000 && echo "ubuntu:ubuntu" | chpasswd && adduser ubuntu sudo
RUN echo "ubuntu ALL= NOPASSWD: ALL\n" >> /etc/sudoers
RUN apt-get install -y -q wget lcov curl
RUN wget https://cmake.org/files/v3.10/cmake-3.10.0-Linux-x86_64.sh
RUN chmod +x cmake-3.10.0-Linux-x86_64.sh && ./cmake-3.10.0-Linux-x86_64.sh --skip-license --prefix=/usr
RUN cmake --version

USER ubuntu
WORKDIR /home/ubuntu
