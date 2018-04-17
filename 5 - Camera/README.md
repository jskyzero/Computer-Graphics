## 5 - Camera
`jskyzero` `2018/04/09`

## Overview

<!-- ![](./docs/output.gif) -->
<video id="video" controls="" preload="none">
    <source id="mp4" src="./docs/output.mp4" type="video/mp4">
    <p>Your user agent does not support the HTML5 Video element.</p>
</video>

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
    MsBuild.exe homework.sln
    ```
  3. run the output binary file
+ If there are questions or errors
  1. if run in windows have null function error, try to comment the funciton `glBindSampler()` in `src/imgui_impl_glfw_gl3.cpp`
  2. ...

## ModelView Matrix

> 在现实生活中，我们一般将摄像机摆放的空间View matrix和被拍摄的物体摆设的空间Modelmatrix分开，但是在OpenGL中却将两个合二为一设为ModelView matrix，通过上面的作业启发，你认为是为什么呢？在报告中写入。（Hints：你可能有不止一个摄像机）

1. 变换矩阵越少计算量越小
2. 在三次变换中至少有一次停止，而在变换到摄像机坐标系时停止不会遇到数值的准确度问题。

## Structure

> 简要说明实现思路以及主要function/algorithm的解释

+ 实现思路
  + 放置在(-1.5, 0.5, -1.5)位置: 修改Model的变换矩阵。
  + 求6个面颜色不一致：不增加新的输入数据的话，简单的就根据每个顶点生成不同颜色。
  + 正交投影，透视投影：设置成可以输入数据，因此可以自己组合多组输入。
  + 视角变换：修改View矩阵。
+ 主要function/algorithm的解释
为了方便理解这里使用uniform传入3个矩阵
```
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
    ourColor = aPos;
}
```
就根据输入的数据设置对应的矩阵。
```
if (orthographic_projection) {
      // 多组(left, right, bottom, top, near, far)
      projection = glm::ortho(orth_left, orth_right, orth_bottom, orth_top,
                              orth_near, orth_far);
    }
    if (perspective_projection) {
      projection =
          glm::perspective(glm::radians(pers_fov), (float)width / (float)height,
                           pers_near, pers_far);
    }
    // if set view
    if (view_changing) {
      model = glm::mat4(1.0f);
      float radius = 10.0f;
      float camX = sin(glfwGetTime()) * radius;
      float camZ = cos(glfwGetTime()) * radius;
      view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0),
                         glm::vec3(0.0, 1.0, 0.0));
    }
    if (input_move) {
      model = glm::mat4(1.0f);
      // pass projection matrix to shader (note that in this case it could change every frame)
      projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
      // camera/view transformation
      view = camera.GetViewMatrix();
    }
```
