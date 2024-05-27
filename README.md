1、实现了光线和球体的相交和可视化了法线并添加了一个光照。
[photo/normal.PNG](https://github.com/oney2033/Cherno-RayTracing/blob/main/photo/normal.PNG)
2、添加了可交互的3D相机系统，多渲染了一个球体和添加了修改球体位置，半径，颜色的功能。
[photo/towsphere.PNG](https://github.com/oney2033/Cherno-RayTracing/blob/main/photo/towsphere.PNG)
3、添加了光线反射的功能
[photo/reflect.PNG](https://github.com/oney2033/Cherno-RayTracing/blob/main/photo/reflect.PNG)
4、添加了物体的材质和漫反射
[photo/Material.PNG](https://github.com/oney2033/Cherno-RayTracing/blob/main/photo/Material.PNG)
# Walnut

Walnut is a simple application framework built with Dear ImGui and designed to be used with Vulkan - basically this means you can seemlessly blend real-time Vulkan rendering with a great UI library to build desktop applications. The plan is to expand Walnut to include common utilities to make immediate-mode desktop apps and simple Vulkan applications.

Currently supports Windows - with macOS and Linux support planned. Setup scripts support Visual Studio 2022 by default.

![WalnutExample](https://hazelengine.com/images/ForestLauncherScreenshot.jpg)
_<center>Forest Launcher - an application made with Walnut</center>_

## Requirements
- [Visual Studio 2022](https://visualstudio.com) (not strictly required, however included setup scripts only support this)
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home#windows) (preferably a recent version)

## Getting Started
Once you've cloned, run `scripts/Setup.bat` to generate Visual Studio 2022 solution/project files. Once you've opened the solution, you can run the WalnutApp project to see a basic example (code in `WalnutApp.cpp`). I recommend modifying that WalnutApp project to create your own application, as everything should be setup and ready to go.

### 3rd party libaries
- [Dear ImGui](https://github.com/ocornut/imgui)
- [GLFW](https://github.com/glfw/glfw)
- [stb_image](https://github.com/nothings/stb)
- [GLM](https://github.com/g-truc/glm) (included for convenience)

### Additional
- Walnut uses the [Roboto](https://fonts.google.com/specimen/Roboto) font ([Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0))
