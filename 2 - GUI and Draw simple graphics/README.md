# 2 - GUI and Draw simple graphics
`jskzyero` `2018/03/15`

## Overview

![](./docs/2018-03-21-12-43-42.gif)

注：
+ 多个三角形分别和原三角形三边拼合，直观上是一个大三角形。
+ gif可能无法播放，请参考`./docs/2018-03-21-12-43-42.gif`

## Install & Run

+ Please see the [PlayGround Example](https://github.com/jskyzero/Computer-Graphics/tree/master/projects/OpenGL-Playground)
+ Can both run in linux and windows
  1. Update the OpenGL library Path in `CMakeLists.txt`, please make sure this library structured like playground example.(it shoulld include glfw/glad/imgui)
  2. Run the fellow commands
    ```Bash
    mkdir build
    cmake ..
    # linux
    make
    # windows
    MsBuild.exe homework.sln
    ```
  3. run the output binary file
+ If there are questions or errors
  1. if run in windows have null function error, try to comment the funciton `glBindSampler()` in `src/imgui_impl_glfw_gl3.cpp`
  2. ...

## Interpolation
> 解释为什么会出现这样的结果（彩色的三角形）

这是片段着色器中进行片段插值的结果，我们只指定了三个定点的颜色，其余的颜色会根据定点的颜色进行插值。

## Structure
> 简要说明实现思路以及主要function/algorithm的解释

+ 实现思路
  + 将一个程序分为初始设置部分，顶点输入和着色器部分，循环渲染部分。
  + 对要求展现的多个图形完成这些部分
  + 加入UI，完成UI逻辑
+ 主要function/algorithm的解释
  + helper部分
    + 初始化部分，负责初始化设置
    + 定点输入和着色器部分，负责编译着色器，VAO/VBO/EBO的buffer和绑定关系
    + 复制函数部分，负责检查操作是否成功，提供文件读写等基本逻辑操作
  + main部分
    + 调用helper，补充不同图形不同的逻辑单元，用函数做参数传递逻辑单元。
    + 全局公用一个VBO，但不同图形VAO使用不同EBO
