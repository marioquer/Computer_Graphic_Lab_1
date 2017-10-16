cmake_minimum_required(VERSION 3.7)
project(CG2017_LAB01)

add_subdirectory(gl3w)

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)

# GL3W wrapper loader setting.
set(GL3W_SRC ${CMAKE_CURRENT_BINARY_DIR}/gl3w.c)
add_custom_command(OUTPUT ${GL3W_SRC}
	COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/gl3w/src/gl3w.c ${GL3W_SRC}
	VERBATIM)
set(GL3W_INC gl3w/include)

# Source and target settings.
set(CGLAB01_SRC src/glmain.cpp 
	src/jpegld.cpp src/glshader.cpp)
set(CGLAB01_INC include)
set(CGLAB01_EXE CG2017_LAB01)

# Add the main executables.
include_directories(${CGLAB01_INC} ${GL3W_INC})
add_executable(${CGLAB01_EXE} 
	${CGLAB01_SRC} ${GL3W_SRC})
add_dependencies(${CGLAB01_EXE} gl3w)
target_link_libraries(${CGLAB01_EXE} 
	dl glfw GL GLU jpeg)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
