@echo off
echo Compiling all GLSL shaders to SPIR-V...

:: Compile all .vert files
for /R %%f in (*.vert) do (
    echo Compiling %%f ...
    glslangValidator -V "%%f" -o "%%f.spv"
)

:: Compile all .frag files
for /R %%f in (*.frag) do (
    echo Compiling %%f ...
    glslangValidator -V "%%f" -o "%%f.spv"
)

:: Compile all .task files
for /R %%f in (*.task) do (
    echo Compiling %%f ...
    glslangValidator -V "%%f" -o "%%f.spv" --target-env vulkan1.2
)

:: Compile all .mesh files
for /R %%f in (*.mesh) do (
    echo Compiling %%f ...
    glslangValidator -V "%%f" -o "%%f.spv" --target-env vulkan1.2
)

:: Compile all .comp files
for /R %%f in (*.comp) do (
    echo Compiling %%f ...
    glslangValidator -V "%%f" -o "%%f.spv"
)

echo Done.
pause
