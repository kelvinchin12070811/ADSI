# Artwork Digital Signature Identifier

Artwork Digital Signature Identifier is an experiment that try to sign a digital artwork with Asynmetric Encryption.
This project is prepared for Final Year Project for SEGi University

System Requirements Specification Form:
    [form](https://www.dropbox.com/s/vjlsmh3m7rksrfe/FYP1_RequirementsSpecification%20_Kaiwen-signed.pdf?dl=0)

FYP Report:
    [report](https://www.dropbox.com/s/fesp098tyc5cfdo/FYP2%20Project%20Report%20-%20Student%20Signed.pdf?dl=0)

## Installation

The project is configured with cmake and vcpkg.

### Required library

 - [Boost](https://www.boost.org/)
 - [Crypto++](https://www.cryptopp.com/)
 - [KF5WidgetsAddons](https://api.kde.org/frameworks/kwidgetsaddons/html/index.html)
 - [libbcrypt](https://github.com/trusch/libbcrypt)
 - [Qt5](https://www.qt.io/)
 - [sqlite_orm](https://github.com/fnc12/sqlite_orm)
 - [SQLite3](https://www.sqlite.org)

### Setup Qt

Get Qt 5.15.2 from the [Official Qt website](https://www.qt.io)

### Setup vcpkg and install required libraries

> Note: Install vcpkg require git installed.

 1. install vcpkg with the following command.

    PowerShell
    ```
    git clone https://github.com/microsoft/vcpkg.git
    Set-Location vcpkg
    ./bootstrap-vcpkg
    ./vcpkg integrate install
    ```

    cmd
    ```
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg
    ./vcpkg integrate install
    ```

 1. Install required library

    ```
    ./vcpkg install cryptopp boost sqlte3 sqlite-orm ecm
    ```

 1. kf5widgetsaddons require additional steps

    > Note: If install Qt directly from vcpkg will not require this steps, install them directly with vcpkg.
    
    1. Open ./vcpkg/ports/kf5widgetsaddons/CONTROL

    1. make change of the following line

        ```
        Build-Depends: ecm, qt5-base, qt5-tools
        ```

        to

        ```
        Build-Depends: ecm
        ```

    1. install kf5widgetsaddons as following

        ```
        ./vcpkg install kf5widgetsaddons --editable
        ```

        The build should failed as no Qt library found.

    1. Head to ./vcpkg/buildtrees/kf5widgetsaddons and delete all files instead of src folder

    1. Open ./src/{version}/CMakeLists.txt and add the following line right after the cmake_minimum_required line
        ```
        list(APPEND CMAKE_PREFIX_PATH "{Path to qt toolkit}")
        ```

        example
        ```
        list(APPEND CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64")
        ```

    1. Run `./vcpkg install kf5widgetsaddons --editable` again, might need to change `REQUIRED_QT_VERSION` to match Qt
        installation version at CMakeLists.txt as previous steps.

 1. Setup vcpkg for cmake in IDE of choice, take a look at
    [vcpkg official documentation](https://github.com/microsoft/vcpkg#using-vcpkg-with-cmake)

## License
[MPL 2.0](https://www.mozilla.org/en-US/MPL/2.0/)
