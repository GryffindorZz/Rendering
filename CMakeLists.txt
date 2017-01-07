cmake_minimum_required(VERSION 3.6)
project(Rendering)

set(CMAKE_CXX_STANDARD 11)
set(USRLIBPATH /usr/local/lib/)
set(SYSLIBPATH /System/Library/Frameworks/)

set(SOURCE_FILES main.cpp Shader.h Shader.cpp Model.h Model.cpp MyMesh.h Light.h Light.cpp Camera3D.h Camera3D.cpp)
add_executable(Rendering ${SOURCE_FILES})
target_link_libraries(Rendering ${USRLIBPATH}libGLEW.a ${USRLIBPATH}libglfw.dylib ${USRLIBPATH}libOpenMeshCore.dylib ${USRLIBPATH}libOpenMeshTools.dylib ${SYSLIBPATH}OpenGL.framework/Libraries/libGL.dylib)