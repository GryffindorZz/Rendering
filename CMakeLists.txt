cmake_minimum_required(VERSION 3.6)
project(Rendering)

set(CMAKE_CXX_STANDARD 11)

FIND_LIBRARY(COCOA_LIBRARY Cocoa)
FIND_LIBRARY(IOKit_LIBRARY IOKit )
FIND_LIBRARY(COREVIDEO_LIBRARY CoreVideo )
FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
MARK_AS_ADVANCED (COCOA_LIBRARY IOKit_LIBRARY COREVIDEO_LIBRARY OPENGL_LIBRARY)
SET(EXTRA_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${COREVIDEO_LIBRARY} ${OPENGL_LIBRARY})
LINK_DIRECTORIES(/usr/local/lib/)
include_directories(${CMAKE_SOURCE_DIR}/ext)
set(SOURCE_FILES main.cpp Shader.h Shader.cpp Model.h Model.cpp MyMesh.h Light.h Light.cpp Camera3D.h Camera3D.cpp
        imgui_impl_glfw_gl3.h imgui_impl_glfw_gl3.cpp
        ./ext/imgui.h ./ext/imgui.cpp ./ext/imconfig.h ./ext/imgui_demo.cpp ./ext/imgui_draw.cpp ./ext/imgui_internal.h
        ./ext/stb_rect_pack.h ./ext/stb_textedit.h ./ext/stb_truetype.h Interaction.h Interaction.cpp)
add_executable(Rendering ${SOURCE_FILES})
target_link_libraries(Rendering libGLEW.a libglfw.dylib libnfd.a
        libOpenMeshCore.dylib libOpenMeshTools.dylib ${EXTRA_LIBS})