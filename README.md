# The Base Framework

A framework for developing platform independent applications in C++.

Copyright (C) 2000-2019 by René Møller Fonseca <rene.m.fonseca+base@gmail.com>

Primary hosting at https://dev.azure.com/renefonseca/base



## Status

[![Windows x64](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=windows)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Windows x86](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=windows_x86)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Windows ARM64](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=windows_arm64)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Windows ARM32](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=windows_arm32)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Ubuntu 18.04](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=ubuntu18_04)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![Ubuntu 16.04](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=ubuntu16_04)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)
[![macOS](https://dev.azure.com/renefonseca/base/_apis/build/status/rene-fonseca.base?branchName=master&jobName=macos)](https://dev.azure.com/renefonseca/base/_build/latest?definitionId=5&branchName=master)



## Dependencies

ZLib, BZip2, OpenGL, and X11 are optional packages. For Ubuntu install:

```shell
sudo apt-get install zlib1g-dev
sudo apt-get install libbz2-dev
sudo apt-get install libx11-dev
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
make
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
make
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
macOS              | x86_64   | Clang   | Ok         | Runs continuously on pipeline
Windows            | x86      | MSC     | Ok         | 8 Dec 2019
Windows            | ARM32    | MSC     | Ok         | 8 Dec 2019
Windows            | ARM64    | MSC     | Ok         | 8 Dec 2019
Debian 10          | x86_64   | GCC     | Ok         | 5 Dec 2019
Ubuntu 19.04       | x86_64   | GCC     | Ok         | 5 Dec 2019
Fedora 31          | x86_64   | GCC     | Ok         | 5 Dec 2019
RedHat 8.1         | x86_64   | GCC     | Ok         | 7 Dec 2019
CentOS 8           | x86_64   | GCC     | Ok         | 6 Dec 2019
FreeBSD 12.1       | x86_64   | Clang   | Ok         | 7 Dec 2019
OpenBSD 66         | x86_64   | GCC     | Ok         | 9 Dec 2019
Raspbian 10        | ARM32    | GCC     | Ok         | 9 Dec 2019
openSUSE 15        | x86_64   | GCC     | Ok         | 9 Dec 2019
Amazon Linux 2     | x86_64   | GCC     | Ok         | 17 Dec 2019 - Requires cmake build
Emscripten         | WASM32   | LLVM    | Ok         | 20 Dec 2019
OpenSolaris 11     | x86_64   | GCC     | Ok         | 6 Dec 2019
OpenSolaris 11     | x86_64   | LLVM    | Pending    |
OpenSolaris 11     | x86_64   | Studio  | Pending    |
OpenSolaris 11     | sparc    |         | Pending    |
Linux              | ARM32    |         | Pending    |
Linux              | ARM64    |         | Pending    |
HP-UX 11i v2       |          |         | Pending    |
AIX 7.2            | PowerPC  |         | Pending    |
RHEL 8             | PowerPC  |         | Pending    |
SUSE Linux 15      | x86_64   |         | Pending    |
WebAssembly WASI   | WASM32   | LLVM    | Build      | 14 Dec 2019 - Only builds but does not run
IOS                | ARM64    | Clang   | Build      | 18 Dec 2019 - Not run
GNU/Linux          | PowerPC  | GCC     | Build      | 23 Dec 2019 - Not run
GNU/Linux          | sparc64  | GCC     | Build      | 23 Dec 2019 - Not run
GNU/Linux          | s390x    | GCC     | Build      | 23 Dec 2019 - Not run
GNU/Linux          | ARM64    | GCC     | Build      | 23 Dec 2019 - Not run
