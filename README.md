# Sourcetrail

Sourcetrail is a free and open-source cross-platform source explorer that helps you get productive on unfamiliar source code.

[![Build](https://github.com/OpenSourceSourceTrail/Sourcetrail/actions/workflows/build.yml/badge.svg)](https://github.com/OpenSourceSourceTrail/Sourcetrail/actions/workflows/build.yml)

__Links__
* [Download](https://github.com/OpenSourceSourceTrail/Sourcetrail/releases)
* [Quick Start Guide](DOCUMENTATION.md#getting-started)
* [Documentation](DOCUMENTATION.md)
* [Changelog](CHANGELOG.md)

!["Sourcetrail User Interface"](docs/readme/user_interface.png "Sourcetrail User Interface")

Sourcetrail is:
* free
* working offline
* operating on Windows, macOS and Linux
* supporting C, C++, Java and Python
* offering an SDK ([SourcetrailDB](https://github.com/CoatiSoftware/SourcetrailDB)) to write custom language extensions

## Sourcetrail on Patreon

The open-source development and regular software releases are made possible entirely by the support of [these awesome patrons](SPONSORS.md)!

## Using Sourcetrail

To setup Sourcetrail on your machine, you can either download the respective build for your operating system from our list of [Releases](https://github.com/CoatiSoftware/Sourcetrail/releases) and install it on your machine, or use one of the following package managers:

* Use the [Chocolatey package](https://chocolatey.org/packages/sourcetrail) with `choco install sourcetrail`

After your installation is complete, follow our [Quick Start Guide](DOCUMENTATION.md#getting-started) to get to know Sourcetrail.

## How to Report Issues

You can post all your feature requests and bug reports on our [issue tracker](https://github.com/CoatiSoftware/Sourcetrail/issues).

### Reporting

Use the following template:

* platform version:
* Sourcetrail version:
* description of the problem:
* steps to reproduce the problem:

### Supporting

If you want to support a certain feature request or you have the same bug that another user already reported, please let us know:
* post a comment with "+1" to the issue
* or send an email to support@sourcetrail.com with the issue ID

## How to Contribute

* Please read and follow the steps in [CONTRIBUTING.md](CONTRIBUTING.md) file.
* You may want to look out for issues labeled [good first issue](https://github.com/CoatiSoftware/Sourcetrail/issues?q=is%3Aopen+is%3Aissue+label%3A%22good+first+issue%22) to find some initial tasks to tackle.
* If you are looking for more information about Sourcetrail software development, please refer to our [wiki](https://github.com/CoatiSoftware/Sourcetrail/wiki).

# How to Build

Building Sourcetrail requires several dependencies to be in place on your machine. However, our CMake based setup allows to disable indexing support for specific languages which reduces the number of dependencies to a minimum.

## Building the Base Application

### Required Tools

* __CMake v3.20 (required for Windows, Linux and MacOS)__
    * __Reason__: Used to generate a build configuration for your build system
    * __Download__: https://cmake.org/download

* __Git (required for Windows, Linux and MacOS)__
    * __Reason__: Used for version control and to automatically generate the Sourcetrail version number from commits and tags
    * __Download__: https://git-scm.com/download
    * __Remarks__: Make sure `git` is added to your `PATH` environment variable before running CMake

* __Visual Studio (required for Windows)__
    * __Reason__: Used for building Sourcetrail
    * __Download__: https://visualstudio.microsoft.com/downloads/

* __ccache (optional for Linux and MacOS)__
    * __Reason__: Used to speed up rebuilds if found in `PATH`
    * __Download__: https://ccache.dev/download.html

### Required dependencies

* __Boost 1.67__
    * __Reason__: Used for file system access and interprocess communication
    * __Prebuilt Download for Windows__: https://sourceforge.net/projects/boost/files/boost-binaries/
    * __Building for Unix__:
        ```
        $ ./bootstrap.sh --with-libraries=filesystem,program_options,system,date_time
        $ ./b2 --link=static --variant=release --threading=multi --runtime-link=static --cxxflags=-fPIC
        ```

* __Qt 5.15__
    * __Reason__: Used for rendering the GUI and for starting additional (indexer) processes.
    * __Prebuilt Download__: http://download.qt.io/official_releases/qt/

### Building

#### On Windows
* To set up your build environment run:
    ```
    $ git clone https://github.com/CoatiSoftware/Sourcetrail.git
    $ cd Sourcetrail
    $ mkdir -p build/win64
    $ cd build/win64
    $ cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT=<path/to/boost_1_67_0> -DQt5_DIR=<path/to/Qt/version/platform/compiler/lib/cmake/Qt5> ../..
    ```
    _Hint: If you are using the CMake GUI, we recommend that you activate advanced mode. Also you may be required to add some of the defines via the "Add Entry" button._

* After generating the build configuration, just open the Sourcetrail.sln file that was generated by CMake and build the Sourcetrail project.

#### On Unix

* To set up your build environment run:
    ```
    $ cd Sourcetrail
    $ mkdir -p build/Release
    $ cd build/Release
    $ cmake -DCMAKE_BUILD_TYPE="Release" -DBOOST_ROOT=<path/to/boost_1_67_0> -DQt5_DIR=<path/to/Qt/version/platform/compiler/lib/cmake/Qt5> ../..
    ```
* Now start the build with:
    ```
    $ make Sourcetrail
    ```

### Running

* Run Sourcetrail from within the build directory. During execution Sourcetrail needs resources from `bin/app/data` and `bin/app/user`. CMake creates symlinks within the build directory that make these directories accessible.


## Enable C/C++ Language Support

### Required dependencies

* __LLVM/Clang 15.0.7__
    * __Reason__: Used for running the preprocessor on the indexed source code, building and traversing an Abstract Syntax Tree and generating error messages.
    * __Building__: Make sure to check out the correct tag: `git checkout llvmorg-15.0.7`
    * __Building for Windows__: Follow [these steps](https://clang.llvm.org/get_started.html) to build the project. Run the cmake command exactly as described. Make sure to build with `-DLLVM_ENABLE_PROJECTS:STRING=clang -DLLVM_ENABLE_RTTI:BOOL=ON -DLLVM_TARGETS_TO_BUILD=host`.
    * __Building for Unix__: Follow this [installation guide](http://clang.llvm.org/docs/LibASTMatchersTutorial.html) to build the project. Make sure to build with `-DLLVM_ENABLE_PROJECTS:STRING=clang -DLLVM_ENABLE_RTTI:BOOL=ON -DCLANG_LINK_CLANG_DYLIB:BOOL=ON -DLLVM_LINK_LLVM_DYLIB:BOOL=ON -DLLVM_TARGETS_TO_BUILD=host`.

### Building

* Run CMake with these additional options:
    ```
    -DClang_DIR=<path/to/llvm_build>/lib/cmake/clang
    -DBUILD_CXX_LANGUAGE_PACKAGE=ON
    ```
* Build Sourcetrail as described [above](#building).

## Creating the deployment Package

### Windows

#### Required Tools

`Moving to qt installer framework`

* __Visual Studio (required for Windows)__
    * __Reason__: Used for building the Sourcetrail Windows installer.
    * __Remarks__: Make sure to install the `.Net desktop development` workload.
    * __Download__: https://visualstudio.microsoft.com/downloads/

* __Wix 3.11__
    * __Reason__: Used to build the `sourcetrail.msi` Windows installer.
    * __Remarks__: Make sure to add `<path/to>/WiX Toolset v3.11/bin` to your `PATH` environment variable.
    * __Download__: https://wixtoolset.org/releases/

* __Wix extension for Visual Studio__
    * __Reason__: Used to run Wix from the Visual Studio build environment.
    * __Download__: https://marketplace.visualstudio.com/items?itemName=WixToolset.WixToolsetVisualStudio2017Extension


#### Building

* Run `./script/deploy_windows.sh` from your Developer Command Prompt for Visual Studio. The script which will generate a 64bit build and packages it into a portable `.zip` file and a Wix-based Windows installer, each.

### macOS

After building, run the `bundle_install.sh` script within the build directory which will create a `Sourcetrail.app` bundle and generate a `Sourcetrail_<version>.dmg` container.

### Linux

Run `./setup/Linux/createPackages.sh` from the main directory, which creates both a `.tar.gz` and a `.AppImage` package in the main directory. Packaging depends on [linuxdeployqt](https://github.com/probonopd/linuxdeployqt).


### How to Run the Tests

The automated test suite of Sourcetrail is powered by [GTest](https://github.com/google/googletest). To run the tests, simply execute the `ctest`.

### License

Sourcetrail is licensed under the [GNU General Public License Version 3](LICENSE.txt).

### Trademark

The "Sourcetrail" name is a trademark owned by Coati Software and is not included within the assets licensed under the GNU GPLv3.
