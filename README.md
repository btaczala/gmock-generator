**Google Mock** mocks generator based on libclang

### Requirements
 + c++14 compiler (clang, gcc)
 + [libclang](http://clang.llvm.org) (tested with 3.9)

### Download
```
git clone --recursive git@github.com:krzysztof-jusiak/gmock.git
```

### Building
```
cd gmock-generator
mkdir build && cmake .. && make
```

### Usage

```sh
$ ./gmock-generator --help
mock files from interface
Usage:
  ./gmock-generator [OPTION...] header files

  -o, --stdout  Print generated gmock file on stdout
  -h, --help    Print this help

```


### Example
```sh
./gmock-generator file.hpp
```

### License
Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).

