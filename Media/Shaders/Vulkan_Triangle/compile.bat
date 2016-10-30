@echo off 
setlocal enabledelayedexpansion

cd %CD%

rem for %%i in (*.vert) do glslangValidator.exe %%i -S "vert" -V -o %%i.spv
rem for %%i in (*.frag) do glslangValidator.exe %%i -S "frag" -V -o %%i.spv


for %%a in (*.glsl.vs) do ( 
	set oldName=%%a 
	set newName=!oldName:glsl=spir-v! 
	glslangValidator.exe !oldName! -S "vert" -V -o !newName!
) 

for %%a in (*.glsl.fs) do ( 
	set oldName=%%a 
	set newName=!oldName:glsl=spir-v! 
	glslangValidator.exe !oldName! -S "frag" -V -o !newName!
) 


pause