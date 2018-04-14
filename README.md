## 6 - Lights and Shading
`jskyzero` `2018/04/12`

## Overview

<!-- ![](./docs/output.gif) -->
注：
+ gif可能无法播放，请参考`./docs/output.gif`
+ 本次gif文件过大，插入了视频文件

## Install & Run

+ Please see the [PlayGround Example](https://github.com/jskyzero/Computer-Graphics/tree/master/projects/OpenGL-Playground)
+ Can both run in linux and windows
  1. Update the OpenGL library Path in `CMakeLists.txt`, please make sure this library structured like playground example.(it shoulld include glfw/glad/imgui)
  2. Run the fellow commands
    ```Bash
    mkdir build
    cd build
    cmake ..
    # linux
    make
    # windows
    MsBuild.exe homework2.sln
    ```
  3. run the output binary file
+ If there are questions or errors
  1. if run in windows have null function error, try to comment the funciton `glBindSampler()` in `src/imgui_impl_glfw_gl3.cpp`
  2. ...

## Structure

> 简要说明实现思路以及主要function/algorithm的解释

+ 实现思路
+ 主要function/algorithm的解释