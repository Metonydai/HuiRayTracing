@echo off
echo Compiling all GLSL shaders to SPIR-V...

:: Compile all .vert files
for %%f in (*.vert) do (
    echo Compiling %%f ...
    glslangValidator -V "%%f" -o "%%f.spv"
)

:: Compile all .frag files
for %%f in (*.frag) do (
    echo Compiling %%f ...
    glslangValidator -V "%%f" -o "%%f.spv"
)

:: Compile all .comp files
for %%f in (*.comp) do (
    echo Compiling %%f ...
    glslangValidator -V "%%f" -o "%%f.spv"
)

echo Done.
pause