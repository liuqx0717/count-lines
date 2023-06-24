:: Usage: test-all [<additional_cmake_args>...]
:: Example:
::    test-all -DTCLAP_INCLUDE_DIR=D:\codes\tclap-1.2.5\include


rmdir /s /q build
mkdir build
cd build
cmake %* ..
cmake --build . -j8 --config Debug
ctest -C Debug
cmake --build . -j8 --config Release
ctest -C Release
cd ..

