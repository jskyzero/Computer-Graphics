## 6 - Lights and Shading
`jskyzero` `2018/04/12`

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
  + Run the fellow commands
  ```Bash
  mkdir build; cd build; cmake ..;
  # linux
  make
  # windows
  MsBuild.exe homework.sln
  ```
  + Run the output binary file
+ If there are questions or errors
  1. if run in windows have null function error, try to comment the funciton `glBindSampler()` in `src/imgui_impl_glfw_gl3.cpp`
  2. ...


## Phong Shading 和 Gouraud Shading

+ Gouraud也就是只计算定点的光照情况，其余点采用插值计算，故计算位置在`vs.shader`中。
+ 其余对局部光照模型的理解基本相同。
  + 环境光：`ambientStrength * lightColor`
  + 漫反射：与法向量和入射角（光源位置）夹角相关
  + 镜面光：与视角和反射光线角度相关

## Structure

> 简要说明实现思路以及主要function/algorithm的解释

+ 实现思路
  + 绘制两个立方体，一个代表光源。
  + 通过大量设置shader中的uniform变量来传递信息
+ 主要function/algorithm的解释。
  + 动态计算法向量：
  ```C++
  auto add_normal_verctor = [](std::vector<GLfloat>& v, int n) {
    glm::vec3 sides[3] = {glm::vec3()};
    for (int i = 0; 3 * (n + 3) * i < (int)v.size(); i++) {
      auto index = (i * (3 * (n + 3)));
      auto p = v.data() + index;
      for (int j = 0; j < 3; j++) {
        sides[j] = glm::vec3(p[n * j + 0], p[n * j + 1], p[n * j + 2]);
      }
      auto normal =
          glm::normalize(glm::cross(sides[2] - sides[1], sides[0] - sides[1]));
      std::vector<GLfloat> normal_v{normal[0], normal[1], normal[2]};
      for (int j = 0; j < 3; j++) {
        v.insert(v.begin() + (index + (n + 3) * (j + 1) - 3), normal_v.begin(),
                 normal_v.end());
      }
    }
  ```
  + shader中计算光照情况
  ```glsl
  vec3 ambient = ambientStrength * lightColor;

  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diffuseStrength * diff * lightColor;

  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;

  vec3 result = lightStrength * (ambient + diffuse + specular) * objectColor;
  FragColor = vec4(result, 1.0);
  ```
