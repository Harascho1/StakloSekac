mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
mingw32-make -j4

cd ..;cd build;mingw32-make;cd ..;cd Program;./StakloRezac.exe

https://en.wikipedia.org/wiki/Guillotine_cutting : wikipedija