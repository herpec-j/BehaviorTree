mkdir Build
cd Build
cmake .. -G "Visual Studio 12"
cmake --build .
ctest .