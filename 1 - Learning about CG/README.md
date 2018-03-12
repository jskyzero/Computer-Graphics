# 1 - Learning about CG

1. 结合上述参考链接（或其他参考资料），谈谈自己对计算机图形学的理解。

在没有接触课程以前，我觉得计算机图形学是研究图像在计算机上的显示的，在接触课程以后，用正式点的语言来描述的话，计算机图形学是使用数学算法讲二维/三维图形转化为计算机显示器的栅格形式的科学，具体来说，研究如何使用计算机来展现图形，进行图像的计算，处理中的原理的算法。

在划分上，可以分为建模，渲染，动画等方面的内容。

2. 结合上述参考链接（或其他参考资料），回答什么是OpenGL? OpenGL ES? Web GL? Vulkan? DirectX?
+ OpenGL：由Khronos组织制定并维护的规范，提供一些操作图形图像的标准化的接口。
+ OpenGL ES：OpenGL for Embedded Systems，指为手机，PDA，游戏主机等嵌入式设备设计的OpenGL的部分API。
+ Web GL：WebGL是一种面向浏览器的渲染3D图像的JavaScript API，基于OpenGL ES。
+ Vulkan：Vulkan是一种低开销，跨平台的3D图像和计算API，目标是全平台的电子游戏或交互式媒体等需要高性能的即时3D图像应用，最初由Khronos在GDC 2015中提出。
+ DirectX：DirectX是一组处理关于多媒体尤其是游戏编程和视频的API集合。

3. gl.h glu.h glew.h 的作用分别是什么?

+ gl: This is the base header file for OpenGL version 1.1. That means if you want to use any functionality beyond version 1.1, you have to add any extension library on this.
+ glew: OpenGL Extension Wrangler Library. This is a cross-platform library for loading OpenGL extended functionality. When you initialize this library, it will check your platform and graphic card at run-time to know what functionality can be used in your program.
+ glu: This is OpenGL utilities library, which has been not updated for long time. Don't need to use this header file.

4. 使用GLFW和freeglut的目的是什么？

+ glfw: OpenGL Frame Work. Another multi-platform library for creating windows and handling events. FreeGlut can be used as an alternative. glfw is designed for game development.
+ freeglut: freeglut is alternative to the OpenGL Utility Toolkit (GLUT) library, since GLUT is getting old and really needs improvement.

5. 结合上述参考链接（或其他参考资料），选择一个SIGGRAPH 2017上 你最喜欢的专题，介绍该专题是做什么的，使用了什么CG技术？（不少于100字）

选择专题：HDR and image manipulation
该专题是做什么的：HDR技术和相对传统的图像的处理
使用了什么CG技术：
