# HuiRayTracing
A famous tutorial *Ray Tracing in One Weekend<ins>* by Peter Shirley. The whole tutorial is raytraced by CPU computing. This project is aimed to use Vulkan API and GLSL code to obtain the raytracing images via compute shader. 

### Method
DataDump.h contains the heads(enum types) and dump(data) like Hittable objects and different Materials, which then dump or memcpy to the uniform buffer and pass to the GPU compute shader.

### Build
Visual Studio 2022 is recommended, focus mainly on a Windows build.

<ins>**1. Downloading the repository:**</ins>
Start by cloning the repository with `git clone --recursive https://github.com/Metonydai/HuiRayTracing`.
If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

<ins>**2. Configuring the Visual Studio solution files:**</ins>
Run the [Win-GenProjects.bat](https://github.com/Metonydai/HuiRayTracing/blob/master/scirpts/Win-GenProjects.bat) file fou file found in scripts folder. This will automatically using the premake5.exe to configure the whole projects.

## Result images
Balls with different materials
![bubble](https://github.com/Metonydai/HuiRayTracing/blob/master/HuiEngine/result_images/bubble.png?raw=true "bubble")
Defocus blurs
![focused_ball](https://github.com/Metonydai/HuiRayTracing/blob/master/HuiEngine/result_images/focusd_ball.png?raw=true "focused_ball")
Noisy Cornell Box
![noisy_cornell_box](https://github.com/Metonydai/HuiRayTracing/blob/master/HuiEngine/result_images/noisy_cornell_box.png?raw=true "noisy_cornell_box")

### References
* [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
* [_Ray Tracing In One Weekend: Vulkan Compute Shader_](https://zhuanlan.zhihu.com/p/463099873)
