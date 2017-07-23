# Build Instructions
## Requirements
- CMake >= 3.0
- Qt5
- VTK 8.0
## CMake
1. Download from https://cmake.org and install. Following buildings are based on it.
## Qt5
1. Download binary SDK from https://www.qt.io/download-open-source/ or http://download.qt.io/official_releases/qt/
1. Install in certain directory
## VTK
1. Download source code from http://www.vtk.org/download/
1. Open CMake(CLI/GUI) and generate project
1. Supplemental Options:
    - BUILD_SHARE_LIBS : On
    - Vtk_Group_Qt : On
    - VTK_QT_VERSION : 5
1. Build in **Release** mode.
1. Install in certain directory.
## Build
1. Open source code and Open CMake(CLI/GUI) and generate project
1. Build in **Release** mode.
## Tips
- Build in short path to prevent path name from being too long in windows
