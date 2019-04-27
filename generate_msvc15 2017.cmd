@rem version vs
@rem SET DEV_TOOLS_MSVC="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"
@rem call %DEV_TOOLS_MSVC% amd64

@rem conan remote add catchorg https://api.bintray.com/conan/catchorg/Catch2
@rem conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan

@SET CMAKE_PREFIX_PATH="qt path"
@mkdir build
@mkdir build\msvc2017x64
@cd build\msvc2017x64
@conan install ..\..\src
@cmake ..\..\src -DCMAKE_PREFIX_PATH=%CMAKE_PREFIX_PATH% -G"Visual Studio 15 2017 Win64"
@pause