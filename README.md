# OpenGL-Playground
`jskyzero` `2017/03/14`

## Overview

a OpenGL envirment that you can play with

### Ubuntu 16.04.2 LTS
```bash
# install OpenGL Library
sudo apt-get install libgl1-mesa-dev
# install GLFW requirement (I don't know if you need this, but I need)
sudo apt-get install xorg-dev libglu1-mesa-dev libx11-dev

# if you want update GLFW/GLAD
# download GLFW to glfw folder
# download GLAD to glad folder

# move to playground dir
cd playground
# make build dir
mkdir build
cd build
# cmake
cmake ..
# make
make
# run 
./src/test.out
```

## Reference
+ [CMake](https://cmake.org/)
+ [GLFW](http://www.glfw.org)
+ [GlAD](http://glad.dav1d.de/)
+ [glfw compile](http://www.glfw.org/docs/latest/compile.html#compile_compile)
+ [glfw build_guide](http://www.glfw.org/docs/latest/build_guide.html)