

Linux: [![Build Status](https://travis-ci.org/w0land/gmock-generator.svg?branch=master)](https://travis-ci.org/w0land/gmock-generator) <br />

Mocks generator based on libclang. 

It can generate mocks based on Google Mocka and Trompeloeil. 

## Requirements
 + c++14 compiler (clang, gcc)
 + [libclang](http://clang.llvm.org) (tested with 3.9)

### Download
```
git clone --recursive git@github.com:w0land/gmock-generator.git
```

### Building
```
cd gmock-generator
mkdir build && cmake .. && make
```

## Usage

```sh
Generates mock files from interface
Usage:
  ./src/mock-generator [OPTION...] header files

  -o, --stdout         Print generated gmock file on stdout
  -s, --strict         Use strict mode
      --hacks          include hacks
  -d, --directory arg  Directory where to store generated gmock (default: $PWD)
  -h, --help           Print this help

```

### Strict Mode

### hacks

### Examples
```sh
./gmock-generator file.hpp
```

```sh
./gmock-generator --strict --hacks -d mocks/ file.hpp
```

```sh
./gmock-generator --strict --hacks -d mocks/ file.hpp file2.hpp file3.hpp
```

## License
Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).

