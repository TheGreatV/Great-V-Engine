@echo off
setlocal EnableDelayedExpansion

cd %CD%

for %%i in (*.vert) do glslangValidator.exe %%i -V -o %%i.spv
for %%i in (*.frag) do glslangValidator.exe %%i -V -o %%i.spv

pause