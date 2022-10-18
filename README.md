# Raytracing in One Weekend++
This repository contains a raytracer built by following Peter Shirley's [Raytracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) guide, with some added functionality from myself.

## Current State of Project
Currently I have gone through Shirley's guide and implemented all the functionality within it. I have since then added some extra features (hence the ++ 😁) and continue to add more. The features I've added thus far are the following:
  - Multithreading support to speed up render times on CPU
  - Video frames rendering (movable camera that can pan, spin, etc. as well as shift focus)
  - Render time measurement
  - Render is shown in a desktop window rather than just written into a file

Here is a video to show off the raytracer and the video rendering capability that I added:

![ezgif com-gif-maker (2)](https://user-images.githubusercontent.com/25702188/186802270-2f00752b-c3bf-47c2-a048-f107ec34be76.gif)

The comments in the commits are quite rich with explanations of concepts, bugs and how I fixed them, more renders, etc. Feel free to have a look.

## Build Instructions
I'm doing this project on a 2020 M1 MacBook Air, 8 GB RAM, running MacOS Monterey.

SDL2 must be installed, which I've installed using Homebrew. To make sure that SDL2 get's picked up by the compiler, I've added the following lines in my `~/.zshrc` file:
```
export CPATH=/opt/homebrew/include
export LIBRARY_PATH=/opt/homebrew/lib
```
I'm compiling using g++ like so:
```
g++ -std=c++17 -lSDL2 main.cpp
```
And running like so:
```
./a.out
```
## Output
Single image renders should produce a `.ppm` file in the same directory as the executable.

You can also use the `render_to_window()` function in the renderer class to display the rendered image in a desktop window.

For rendering videos, make a subfolder called `output`, and the video frames will be rendered into there. To combine the video frames into a video, `cd` into the `output` folder and run the following ffmpeg command:
```
ffmpeg -framerate 30 -i "%01d.ppm" output.mp4
```
I've used 30 fps in the code, and thus the number must be the same in this command.

## More Functionality to Implement
  - Use a video encoding library to stitch the frames into a video from within the program instead of rendering frames as seperate files and having to stitch later through an ffmpeg command
  - Add camera movement of a spiral and helix
  - Add a proper progress bar for video rendering
  - Animate objects within the world
  - Create a scripting language and parser for camera and object movements/animations _(aspirational)_
  - Create a scene viewer that dynamically renders, with camera movements done using mouse _(aspirational)_
  - Implement graphics acceleration using Metal/Vulkan/DirectX _(aspirational)_

## Futher Reading
  - [Accelerated Ray Tracing in One Weekend in CUDA](https://developer.nvidia.com/blog/accelerated-ray-tracing-cuda/)
  - [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
  - [Ray Tracing: The Rest of Your Life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)

## Background
My goal of following Shirley's guide is less to learn computer graphics and more to further my programming skills. I have some minor experience with C, and have done some stuff in Python, but I have never learnt OOP formally. And I have certainly never built any serious software like a raytracer using a low-level systems language like C++. 

Why computer graphics? Why raytracing? Surely I could've picked some other project to tackle if my real goal is just to further my programming skills. The reason I picked computer graphics is because it seems to me like the area in computing that most sits at the intersection of the arts and CS. To this day, the most resonant media experience I've had is my first playthrough of _[The Last of Us](https://en.wikipedia.org/wiki/The_Last_of_Us)_. Pixar movies (_Up_, _WALL-E_, _Inside Out_) are also on the list. Such media, among others (see below) are my inspirations for delving into computer graphics.

List of videos/media/people that inspire me to learn computer graphics:
  - [UNCHARTED 2: Among Thieves™ - Mastering the Cell (BTS #6)](https://www.youtube.com/watch?v=Az0CXkyoWjg)
  - [Tech Throwback: The Untapped Potential of PS3's CELL CPU And How Naughty Dog Mastered It](https://www.youtube.com/watch?v=6nG4YgtIYNA&t=11s)
  - The graphics quality of The Last of Us; the fact that it ran on 2006 hardware
  - Graphics quality of The Last of Us Part II
  - Pixar movies
  - John Carmack
  - Bruce Straley
