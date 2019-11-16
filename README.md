<img src="https://github.com/epezent/carnot/blob/master/misc/carnot_banner.png" width="300">

## Dependencies

[SFML](https://github.com/SFML/SFML)

## Install

Open Powershell (Run as Administrator) in a directory of your choice:

```shell
git clone https://github.com/SFML/SFML
cd SFML
mkdir build
cd build
cmake .. -A x64 -DBUILD_SHARED_LIBS=OFF
cmake --build . --target install --config Release
cmake --build . --target install --config Debug

cd ../..
git clone https://github.com/epezent/carnot
cd carnot
mkdir build
cd build
cmake .. -A x64
cmake --build . --target install --config Release
cmake --build . --target install --config Debug
```

<!-- 
cmake .. -G Ninja -DCMAKE_C_COMPILER:PATH="C:\Program Files\LLVM\bin\clang-cl.exe" -DCMAKE_CXX_COMPILER:PATH="C:\Program Files\LLVM\bin\clang-cl.exe"
 -->