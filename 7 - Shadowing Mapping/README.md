## 7 - Shadowing Mapping
`jskyzero` `2018/05/27`

## Overview

<!-- ![](./docs/output.gif) -->
注：
+ gif可能无法播放，请参考`./docs/output.gif`或者`./docs/output.MP4`

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


## Shadowing Mapping算法
简单的来说，阴影映射算法就是先从光源的角度看一下目标点是否可以看到，然后我们使用一个纹理储存这些信息，然后用这个纹理结合从观察者的角度看到的图像来生成最终的输出效果。

1. 生成深度贴图（从光源的角度测试目标点是否可以看到，用纹理储存）

```c++
  // 生成纹理

  // Configure depth map FBO
  const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
  GLuint depthMapFBO;
  glGenFramebuffers(1, &depthMapFBO);
  // - Create depth texture
  GLuint depthMap;
  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

     //从光源的角度测试目标点是否可以看到
     glm::mat4 lightProjection, lightView;
     glm::mat4 lightSpaceMatrix;
     GLfloat near_plane = 1.0f, far_plane = 7.5f;
     lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
     lightView = glm::lookAt(light_position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
     lightSpaceMatrix = lightProjection * lightView;
     // - render scene from light's point of view
     glUseProgram(depth_shader_program);
     glUniformMatrix4fv(glGetUniformLocation(depth_shader_program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

     glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
     glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
       glClear(GL_DEPTH_BUFFER_BIT);
       RenderScene(depth_shader_program);
     glBindFramebuffer(GL_FRAMEBUFFER, 0);
```

2. 用这个深度贴图结合生成最终的输出效果。

```cpp
	glViewport(0, 0, kScreenWidth, kScreenHeight);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glUseProgram(shadow_shader_program);
     glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)width / (float)height, 0.1f, 100.0f);
     glm::mat4 view = camera->GetViewMatrix();
     helper::SetShaderMat4(shadow_shader_program, "projection", projection);
     helper::SetShaderMat4(shadow_shader_program, "view", view);

     // set light uniforms
     helper::SetShaderVec3(shadow_shader_program, "viewPos", camera->Position);
     helper::SetShaderVec3(shadow_shader_program, "lightPos", light_position);
     helper::SetShaderMat4(shadow_shader_program, "lightSpaceMatrix", lightSpaceMatrix);

     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D, box_texture);
     glActiveTexture(GL_TEXTURE1);
     glBindTexture(GL_TEXTURE_2D, depthMap);
     RenderScene(shadow_shader_program);
```

3. shader 部分

```glsl
// 深度着色器 vs
#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f);
}
```
```glsl
// 阴影着色器 fs
#version 330 core
out vec4 FragColor;

// VS data structure
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

// testure
uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

// light
uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // 测试是否可见
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    // initial color
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);

    // 环境光
    // ambient
    vec3 ambient = 0.3 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    // 计算阴影
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
	// 合成
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    FragColor = vec4(lighting, 1.0);
}
```

   