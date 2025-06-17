# CPU Ray Tracing in C 

## Build and run
This project is a way for me to learn the C programming language. It is configured to build on 
my machine (running ubuntu, compiling with clang). The only dependency is SDL. To run the project 
locally, you will likely have to create your own build system. My system supports building with 
different flags (release, debug, and test) and outputs the resulting file into the target directory.

## Features
This is a lightly featured rendering engine intended as a learning project, and misses many important 
features; however, the basics are here:
- Lambertian diffuse, reflective, and refractive / transparent materials
- Triangles and spheres
- Importing wavefront obj files (up to 650 triangles)
- Depth of field
- Sub-pixel sampling / anti-aliasing
- Bounding boxes for all objects to optimize performance

## Demo
This is a simple demo scene with an imported model car to show off the functionaliry of my renderer.

![Screenshot from 2025-06-17 01-43-12](https://github.com/user-attachments/assets/dde46b86-d6ec-47df-93dd-20ce0327325c)
