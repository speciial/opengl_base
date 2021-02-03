@echo off

mkdir build
pushd build\

cl -nologo -Zi -FC -I ..\include ..\code\application.cpp user32.lib gdi32.lib shell32.lib opengl32.lib ..\lib\glad.lib ..\lib\glfw3.lib

popd