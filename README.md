# Raytracing in One Weekend
This repository is essentially my journey working through [Peter Shirley's Raytracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html).
My goal of following Shirley's guide is less to learn computer graphics and more to further my programming skills. I have some minor experience with C, and have done some stuff in Python, but I have never learnt OOP formally. And I have certainly never built any serious software like a raytracer using a low-level systems language like C++. 

The best way to learn something, in my experience, has been to just do it. Dive straight in and learn along the way, instead of reading a textbook on the thing first and try to make sense of things without actually just doing the thing. With this philosophy in mind, Shirley's guide seemed like an excellent resource to learn by doing.

Of course, when I'm done going through the guide, my code will likely look extremely similar to his, but that isn't the point. The real goal is the learning. I've done a decent chunk of the guide thus far, and it has taken me a couple weeks. How it has went thus far is that I come upon a code block and it's filled with new concepts like operators, classes, constructors, overloading etc. So I have to pause and learn those concepts first, which involves conceptual understanding, as well as writing little test programs on the side to truly understand the concepts. Only then does the code block make sense, and I'm able to write it myself and move on. But this is exactly what I want; I'd much rather learn as I go in this manner than read a boring C++ textbook before writing anything myself.

The last thing I'd like to address is, why computer graphics? Why raytracing? Surely I could've picked some other project to tackle if my real goal is just to further my programming skills. The reason I picked computer graphics is because it seems to me like the area in computing that most sits at the intersection of the arts and CS. To this day, the most resonant media experience I've had is my first playthrough of The Last of Us. Such a masterpiece of art could not be possible without computers and the field of computer graphics. Pixar movies are another example of high art only being possible thanks to computer graphics. I love art, and thus, such is my fascination with computer graphics. I'm eager to learn about it, whilst furthering my programming skills at the same time.

Side note - some videos/people that have inspired me to delve into computer graphics:<br/>
  - [UNCHARTED 2: Among Thieves™ - Mastering the Cell (BTS #6)](https://www.youtube.com/watch?v=Az0CXkyoWjg)<br/>
  - [Tech Throwback: The Untapped Potential of PS3's CELL CPU And How Naughty Dog Mastered It](https://www.youtube.com/watch?v=6nG4YgtIYNA&t=11s)<br/>
  - The graphics quality of The Last of Us; the fact that it ran on 2006 hardware<br/>
  - Graphics quality of The Last of Us Part II<br/>
  - John Carmack<br/>

## To do after finishing
  - Add video rendering functionality (to show off focussing on different objects, for example).
  - Implement multi-threading to speed up render times on CPU
  - Implement graphics acceleration through Metal or DirectX
  - Make a scene viewer that dynamically renders, to avoid having to render one frame at a time as a PPM file
  - Add ability to animate objects in the scene/world

## Further reading
  - https://developer.nvidia.com/blog/accelerated-ray-tracing-cuda/
  - [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
  - [Ray Tracing: The Rest of Your Life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)
