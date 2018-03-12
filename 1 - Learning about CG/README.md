# 1 - Learning about CG

## 1. 结合上述参考链接（或其他参考资料），谈谈自己对计算机图形学的理解。

在没有接触课程以前，我觉得计算机图形学是研究图像在如何在计算机上的显示出来的，在接触课程以后，用正式点的语言来描述的话，计算机图形学是使用数学算法讲二维/三维图形转化为计算机显示器的栅格形式的科学，具体来说，研究如何使用计算机来展现图形，进行图像的计算，处理中的原理的算法。

在划分上，可以分为建模，渲染，动画等方面的内容。

## 2. 结合上述参考链接（或其他参考资料），回答什么是OpenGL? OpenGL ES? Web GL? Vulkan? DirectX?

+ OpenGL：由Khronos组织制定并维护的规范，提供一些操作图形图像的标准化的接口。
+ OpenGL ES：OpenGL for Embedded Systems，指为手机，PDA，游戏主机等嵌入式设备设计的OpenGL的部分API。
+ Web GL：WebGL是一种面向浏览器的渲染3D图像的JavaScript API，基于OpenGL ES。
+ Vulkan：Vulkan是一种低开销，跨平台的3D图像和计算API，目标是全平台的电子游戏或交互式媒体等需要高性能的即时3D图像应用，最初由Khronos在GDC 2015中提出。
+ DirectX：DirectX是一组处理关于多媒体尤其是游戏编程和视频的API集合。

## 3. gl.h glu.h glew.h 的作用分别是什么?

+ gl: This is the base header file for OpenGL version 1.1. That means if you want to use any functionality beyond version 1.1, you have to add any extension library on this.
+ glu: This is OpenGL utilities library, which has been not updated for long time. Don't need to use this header file.
+ glew: OpenGL Extension Wrangler Library. This is a cross-platform library for loading OpenGL extended functionality. When you initialize this library, it will check your platform and graphic card at run-time to know what functionality can be used in your program.

## 4. 使用GLFW和freeglut的目的是什么？

+ glfw: OpenGL Frame Work. Another multi-platform library for creating windows and handling events. FreeGlut can be used as an alternative. glfw is designed for game development.
+ freeglut: freeglut is alternative to the OpenGL Utility Toolkit (GLUT) library, since GLUT is getting old and really needs improvement.

## 5. 结合上述参考链接（或其他参考资料），选择一个SIGGRAPH 2017上 你最喜欢的专题，介绍该专题是做什么的，使用了什么CG技术？（不少于100字）

选择专题：HDR and image manipulation

该专题是做什么的：将LDR图片转化成HDR技术与一些图像的处理的新方法

使用了什么CG技术：
  + Deep Reverse Tone Mapping
    + 使用CNN方法训练模型，对输入图片添加不同的对比度的细节，最后合成成HDR的图片。
  + HDR image reconstruction from a single exposure using deep CNNs
    + 通过估计LDR图片的某些缺失信息，比如光照，来重建HDR的图片。
    + 基于hybrid dynamic range autoencoder方面的CNN。
  + Image Abstraction and Vectorization Using Layered Linear Gradients
    + 用一些抽象的矢量剪切层来拼合输入图片。
    + 将问题转化成每个像素的标签和适应的问题，可能方式太多，无法评估所有的可能方式来寻找最优解，论文的贡献在于使用了随机算法有效的解决了空间分解问题。
  + Learning to Predict Indoor Illumination from a Single Image
    + 提出了自动方法从固定视角的低对比度图片中推断高动态范围的照明。
    + 通过训练好的端到端的深度神经网络直接处理。
  + Transferring Image-based Edits for Multi-Channel Compositing
    + 改进了传统的图像制作的流程，传输用户的编辑操作，从而可以更方便的复用编辑操作。
    + 提出了一套扩展的转化算法。
