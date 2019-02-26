rem version vs
rem SET DEV_TOOLS_MSVC="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"
rem call %DEV_TOOLS_MSVC% amd64
SET CMAKE_PREFIX_PATH="qt path"
SET CMAKE_TOOLCHAIN_FILE="vcpkg path"
mkdir build
mkdir build\msvc2017x64
cd build\msvc2017x64
cmake ..\..\src -DCMAKE_TOOLCHAIN_FILE=%CMAKE_TOOLCHAIN_FILE% -DCMAKE_PREFIX_PATH=%CMAKE_PREFIX_PATH% -G"Visual Studio 15 2017 Win64"
pause