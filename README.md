# Raytracing in One Weekend++
This repository contains a raytracer built following Peter Shirley's [Raytracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) guide. On top of the base code from that guide, I've also added some custom functionality:

  - Multithreading support to speed up render times on CPU
  - Video frames rendering (movable camera that can pan, spin, etc. as well as shift focus)
  - Render time measurement & percentage progress indicator
  - **_Live_** rendering into a desktop window, rather than just a headless render into a file (although that is supported too)

Here's a demo of the video frames rendering and live rendering:

Video Frames Rendering            |  Live Rendering
:-------------------------:|:-------------------------:
<img src="https://user-images.githubusercontent.com/25702188/186802270-2f00752b-c3bf-47c2-a048-f107ec34be76.gif" width="380" />  |  <img src="https://user-images.githubusercontent.com/25702188/197325026-4e6093b0-48e9-4aa0-9a2b-6b7ef8e75603.gif" width="380"/>

This was the first serious software project I did, and the purpose was to teach myself OOP concepts and C++. It was a great first project in that sense, with a very cool and gratifying visual result at the end.

## Build Instructions
Have [SDL2](https://github.com/libsdl-org/SDL/releases), `CMake` and `ninja` installed. Then, after cloning this repo, from the root of the repo, run the following:
```
mkdir build
cd build
cmake -GNinja ../src
ninja
./rt-weekend
```
## Output
Single image renders should produce a `.ppm` file in the same directory as the executable.

You can also use the `render_to_window()` function in the renderer class to see the image get rendered live into a desktop window.

For rendering videos, make a subfolder called `output`, and the video frames will be rendered into there. To combine the video frames into a video, run the following ffmpeg command:
```
ffmpeg -framerate 30 -i "output/%01d.ppm" output.mp4
```
I've used 30 fps in the code, and thus the number must be the same in this command.

## More Functionality to Implement
  - Use a video encoding library to stitch the frames into a video from within the program instead of rendering frames as seperate files and having to stitch later through an ffmpeg command
  - Add camera movement of a spiral and helix
  - Add a proper progress bar for video rendering
  - Implement graphics acceleration using Metal/Vulkan/DirectX _(aspirational)_

## Futher Reading
  - [Accelerated Ray Tracing in One Weekend in CUDA](https://developer.nvidia.com/blog/accelerated-ray-tracing-cuda/)
  - [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
  - [Ray Tracing: The Rest of Your Life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)