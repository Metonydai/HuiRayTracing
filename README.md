# **HuiEngine**

## `master branch`

A famous tutorial *Ray Tracing in One Weekend<ins>* by Peter Shirley. The whole tutorial is raytraced by CPU computing. This project is aimed to use Vulkan API and GLSL code to obtain the raytracing images via compute shader. 

---

### Method
DataDump.h contains the heads(enum types) and dump(data) like Hittable objects and different Materials, which then dump or memcpy to the uniform buffer and pass to the GPU compute shader.

### Build
Visual Studio 2022 is recommended, focus mainly on a Windows build.

<ins>**1. Downloading the repository:**</ins>
Start by cloning the repository with `git clone --recursive https://github.com/Metonydai/HuiRayTracing`.
If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

<ins>**2. Configuring the Visual Studio solution files:**</ins>
Run the [Win-GenProjects.bat](https://github.com/Metonydai/HuiRayTracing/blob/master/scirpts/Win-GenProjects.bat) file fou file found in scripts folder. This will automatically using the premake5.exe to configure the whole projects.

### Result images
Balls with different materials
![bubble](https://github.com/Metonydai/HuiRayTracing/blob/master/HuiEngine/result_images/bubble.png?raw=true "bubble")
Defocus blurs
![focused_ball](https://github.com/Metonydai/HuiRayTracing/blob/master/HuiEngine/result_images/focusd_ball.png?raw=true "focused_ball")
Noisy Cornell Box
![noisy_cornell_box](https://github.com/Metonydai/HuiRayTracing/blob/master/HuiEngine/result_images/noisy_cornell_box.png?raw=true "noisy_cornell_box")

## `mesh_shader branch`

This branch explores real-time rendering techniques using Vulkan **Mesh Shaders** and **Physically Based Rendering (PBR)**.  
It combines cutting-edge GPU features with modern shading workflows to achieve high-performance and visually rich rendering results.

---

###  Overview

This branch introduces a **fully GPU-driven pipeline** leveraging **compute and task/mesh shaders** for dynamic geometry generation.  
Using **marching cubes algorithm** for generating **metaball balls**-complex smooth surfaces are procedurally constructed entirely on the GPU.

In addition, a complete **Image-Based Lighting (IBL)** workflow has been integrated, enabling realistic reflections and lighting via **HDR environment maps**, **irradiance cubemaps**, and **prefiltered environment maps**, topped off with a **BRDF lookup table** for physically-accurate specular reflection.


###  Features
*Mesh Shader Pipeline<ins>*
1. Fully GPU-driven pipeline using **task** and **mesh shaders**.
2. **marching cubes** algorithm geometry generation via compute shader.

*Physically Based Rendering (PBR)<ins>*
1. Realistic material system based on **microfacet BRDF**
2. **IBL integration** for accurate global illumination
3. **HDR texture support** with tone mapping for dynamic exposure control
4. **Irradiance cubemap**, **Prefiltered environment map** and **BRDF LUT** textures precomputed at runtime for efficient rendering

###  Preview
[![metaballs](https://github.com/Metonydai/HuiRayTracing/blob/master/HuiEngine/result_images/metaballs.png?raw=true "metaballs")](https://youtu.be/QiWRTM26xWc)

---


### References
* [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
* [_Ray Tracing In One Weekend: Vulkan Compute Shader_](https://zhuanlan.zhihu.com/p/463099873)
* [_Learn OpenGL_](https://learnopengl.com/)
* [_Vulkan C++ examples and demos_](https://github.com/SaschaWillems/Vulkan)
