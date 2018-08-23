---
layout: default
---
[back](../sc2ai){: .btn.btn-default}

# CMake Support in Visual Studio 2017

[MSDN offcial blog link](https://blogs.msdn.microsoft.com/vcblog/2016/11/16/cmake-support-in-visual-studio-the-visual-studio-2017-rc-update/){: target="_blank"}

In SC2 AI project I use SC2 API from Blizzard with Windows 10. At the very first time I used Ubuntu Linux. However, Blizzard provides only the headless version(without graphics) of SC2 in Linux, so I move to Windows for checking the progress of game with graphics. The project is built by CMake. To understand the whole project structure, it is necessary to understand how CMake support in Visual Studio. 

# What Is CMake
CMake is a open-source tool for cross-platform compilation. The cross-platform means that the environment compiling programs is different from the target environment of the programs. I can easily infer that the reason why Blizzard uses CMake. Blizzard provides SC2 API for Mac, Windows, and Linux. I can check the options for the operating systems in the CMakeList.txt file. I realize that it is necessary to understand how CMake works for knowing the structure of SC2 API.

# The CMake in SC2
The basic of cmake support is that cpp files and a CMakeList.txt file are in the same directory. The CMakeList.txt file includes "include_directories". This is for the source code files to be compiled. Therefore, the result of this program comes from the files defined by "include_directories". With this, project names are defined in to the CMakeList.txt file. With these project name the source code files are paired by "add_executables". Further, it is possible to define functions and use them. I find the source codes of SC2 API examples, and start to implement my own AI. The entry point is CMake information. It is the reason why I explain this, and this is really important.