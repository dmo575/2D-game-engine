# 2D-game-engine
Game engine made in C++ with SDL2.

# WARNING - PLEASE READ:

## **This is a work in progress. I am currently fixing bugs for this project so that it runs well on Linux**

**I originally made this project on Windows, for Windows. I used some windows exclusive libraries to implement the saving and loading. This repo contains a modified version of this project that works on LINUX which is what I am using now. For this Linux version I got rid of the in-game saving and loading features. Please do not try to save and load while in-game, it won't work and it might break. You can perfectly load the levels from the main menu.**


## To get this running on Linux:
- Install a c++ compiler, we are using gcc
    - `sudo dnf install gcc-c++`
- Install cmake (Im using CMake because this way I don't have to input all the .cpp files in the correct order for compilation to work)
    - `sudo dnf install cmake`
- Install SDL2 development libraries (we will be using SDL2, mixer, ttf and image)
    - `sudo dnf install SDL2-devel SDL2_mixer-devel SDL2_ttf-devel SDL2_image-devel`
    - If for some reason your SDL libraries are not located at `/usr/include/SDL2`, please update the `includePath` property inside **c_cpp_properties.json**  and also the `include_directories` inside the CMakeLists.txt with the correct path.
- Create a build directory inside the project directory
    - `mkdir build`
- Run the cmake command **inside the build directory you just created**. This  will generate CMake files using the CMakeLists.txt on the parent directory.
    - `cd build`
    - `cmake ..`
- Go inside the build folder and run the make command
    - `make`
- Copy the Resources directory inside the build folder.
    - If you are on the root directory:
        - `cp -rf Resources build`
- Inside the build directory, run the application by executing this command:
    - `./app`

# About this project
This was a huge project I did a while ago for a fundamentals of game engines class. I decided to implement all that crossed my mind and ended up building this cool little 2D/2.5D game engine.

I initially made it on Windows using Visual Studio as the IDE, so making it work on Linux with VSCode has been a really fun journey. This is the first compiled application I make work on Linux and it has been a great intro snippet on things like CMake, how libraries work on Linux (no .dlls) and path differences between Linux and Windows.

If you are interested on all the features this engine has, please visit its original post I made about it back when my mind was much fresher on it [HERE](https://arc-gameworks.webflow.io/programming/p-2dengine).

# TODO: ADD VIDEO HERE