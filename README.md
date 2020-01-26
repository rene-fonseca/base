# The Base Framework

A framework for developing platform independent applications in C++.

Copyright (C) 2000-2020 by René Møller Fonseca <rene.m.fonseca+base@gmail.com>

Primary hosting at https://dev.azure.com/renefonseca/base

Reference API documentation is hosted at https://rene-fonseca.github.io/base-api


## Status

[![Windows x64](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=windows&label=Windows%20x64)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Windows x86](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=windows_x86&label=Windows%20x86)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Windows ARM64](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=windows_arm64&label=Windows%20ARM64)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Windows ARM32](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=windows_arm32&label=Windows%20ARM32)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Debian](https://dev.azure.com/renefonseca/base/_apis/build/status/base%20daily?branchName=master&jobName=debian_x64&label=Debian)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=7&branchName=master)
[![Ubuntu 19.10](https://dev.azure.com/renefonseca/base/_apis/build/status/base%20daily?branchName=master&jobName=ubuntu19_10_x64&label=Ubuntu%2019.10)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=7&branchName=master)
[![Ubuntu 19.04](https://dev.azure.com/renefonseca/base/_apis/build/status/base%20daily?branchName=master&jobName=ubuntu19_04_x64&label=Ubuntu%2019.04)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=7&branchName=master)
[![Ubuntu 18.04](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=ubuntu18_04&label=Ubuntu%2018.04)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Ubuntu 16.04](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=ubuntu16_04&label=Ubuntu%2016.04)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![CentOS 8](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=centos8&label=CentOS%208)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Fedora x64](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=fedora_x64&label=Fedora%20x64)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![macOS](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=macos&label=macOS)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![iOS](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=ios&label=iOS)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Ubuntu ARM64](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=ubuntu_aarch64&label=Ubuntu%20ARM64)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Ubuntu PowerPC64LE](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=ubuntu_powerpc64le&label=Ubuntu%20PowerPC64LE)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Ubuntu S390x](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=ubuntu_s390x&label=Ubuntu%20S390x)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Ubuntu SPARC64](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=ubuntu_sparc64&label=Ubuntu%20SPARC64)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Ubuntu RISC-V64](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=ubuntu_riscv64&label=Ubuntu%20RISC-V64)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Ubuntu PA-RISC](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=ubuntu_hppa32&label=Ubuntu%20PA-RISC)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![openSUSE](https://dev.azure.com/renefonseca/base/_apis/build/status/base%20daily?branchName=master&jobName=opensuse_x64&label=openSUSE)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=7&branchName=master)
[![Amazon Linux](https://dev.azure.com/renefonseca/base/_apis/build/status/base%20daily?branchName=master&jobName=amazonlinux_x64&label=Amazon%20Linux)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=7&branchName=master)
[![GCC 9](https://dev.azure.com/renefonseca/base/_apis/build/status/base%20daily?branchName=master&jobName=gcc9&label=GCC%209)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=7&branchName=master)
[![FreeBSD x64](https://dev.azure.com/renefonseca/base/_apis/build/status/base%20daily?branchName=master&jobName=freebsd_x64&label=FreeBSD%2012.1)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=7&branchName=master)
[![Emscripten](https://dev.azure.com/renefonseca/base/_apis/build/status/base%20daily?branchName=master&jobName=emcc&label=Emscripten)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=7&branchName=master)
[![wasm32](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=wasm32&label=WebAssembly%2032)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)


## Dependencies

ZLib, BZip2, XMLSOFT XML, XMLSOFT XSLT, OpenSSL, Curl, OpenGL, and X11 are optional packages.

For Ubuntu install:
```shell
sudo apt-get install zlib1g-dev
sudo apt-get install libbz2-dev
sudo apt-get install libxml2-dev
sudo apt-get install libxslt-dev
sudo apt-get install libssl-dev
sudo apt-get install libcurl4-openssl-dev
sudo apt-get install libx11-dev
```

For FreeBSD run:
```shell
portsnap fetch
portsnap extract
portsnap update
cd /usr/ports/textproc/libxml2
make install clean
cd /usr/ports/textproc/libxslt
make install clean
cd /usr/ports/ftp/curl
make install clean
```

Other optional dependencies:
  Wasmtime-C-API



## Build

Make sure GIT, cmake, make, and compiler (e.g. g++) are installed.

### Debug build

```shell
git clone https://dev.azure.com/renefonseca/base/_git/base
cd base
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .. --config Debug --target install -- -j 4
./testing/unittesting
ctest . -C Debug
```

### Release build

```shell
git clone https://dev.azure.com/renefonseca/base/_git/base
cd base
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .. --config Release --target install -- -j 4
./testing/unittesting
ctest . -C Release
```


## Platforms

List of platforms tested.

OS                 | Arch     | Compiler| Status     | Comment
------------------ | -------- | ------- | ---------- | -------
Windows            | x86_64   | MSC     | Ok         | Runs continuously on pipeline
Windows            | x86      | MSC     | Ok         | Runs continuously on pipeline
Windows            | ARM32    | MSC     | Ok         | Runs continuously on pipeline but no testing
Windows            | ARM64    | MSC     | Ok         | Runs continuously on pipeline but no testing
Ubuntu 18.04       | x86_64   | GCC     | Ok         | Runs continuously on pipeline
Ubuntu 16.04       | x86_64   | GCC     | Ok         | Runs continuously on pipeline
Ubuntu             | ARM64    | GCC     | Ok         | Runs continuously on pipeline
Ubuntu             | PowerPC  | GCC     | Ok         | Runs continuously on pipeline
Ubuntu             | s390x    | GCC     | Ok         | Runs continuously on pipeline
Ubuntu             | SPARC64  | GCC     | Ok         | Runs continuously on pipeline
Ubuntu             | PA-RISC  | GCC     | Ok         | Runs continuously on pipeline
Ubuntu             | RISC-V64 | GCC     | Ok         | Runs continuously on pipeline
CentOS 8           | x86_64   | GCC     | Ok         | Runs continuously on pipeline
Fedora             | x86_64   | GCC     | Ok         | Runs continuously on pipeline
macOS              | x86_64   | Clang   | Ok         | Runs continuously on pipeline
iOS                | ARM64    | Clang   | Ok         | Runs continuously on pipeline but no testing
WebAssembly WASI   | WASM32   | LLVM    | Ok         | Runs continuously on pipeline - Exceptions cause abort()
Debian 10          | x86_64   | GCC     | Ok         | Runs daily on pipeline
Ubuntu 19.10       | x86_64   | GCC     | Ok         | Runs daily on pipeline
Ubuntu 19.04       | x86_64   | GCC     | Ok         | Runs daily on pipeline
openSUSE           | x86_64   | GCC     | Ok         | Runs daily on pipeline
Amazon Linux 2     | x86_64   | GCC     | Ok         | Runs daily on pipeline - using newer cmake build
GCC 9              | x86_64   | GCC     | Ok         | Runs daily on pipeline
Emscripten         | WASM32   | LLVM    | Ok         | Runs daily on pipeline but no testing
FreeBSD 12.1       | x86_64   | GCC     | Ok         | Runs daily on pipeline but no testing
RedHat 8.1         | x86_64   | GCC     | Ok         | 7 Dec 2019
FreeBSD 12.1       | x86_64   | Clang   | Ok         | 7 Dec 2019
OpenBSD 66         | x86_64   | GCC     | Ok         | 9 Dec 2019
Raspbian 10        | ARM32    | GCC     | Ok         | 9 Dec 2019
OpenSolaris 11     | x86_64   | GCC     | Ok         | 6 Dec 2019
OpenSolaris 11     | x86_64   | LLVM    | Pending    |
OpenSolaris 11     | x86_64   | Studio  | Pending    |
OpenSolaris 11     | SPARC64  |         | Pending    |
HP-UX 11i v2       | PA-RISC  |         | Pending    |
AIX 7.2            | PowerPC  |         | Pending    |
RHEL 8             | PowerPC  |         | Pending    |

Docker images are available at: https://hub.docker.com/repository/docker/renefonseca/
