cmake_minimum_required(VERSION 3.12)

project(OceanWeather)

# Enable c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
  add_compile_definitions("_GLIBCXX_DEBUG")
endif()

if(MSVC)
  add_definitions(/MP)
endif()

set(CMAKE_CXX_FLAGS "-Wall")
if(WIN32)
set(CMAKE_CXX_FLAGS_DEBUG "/ZI /Od")
set(CMAKE_CXX_FLAGS_RELEASE "/O2")
else(WIN32)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif(WIN32)

include(cmake/opengl.cmake)
include(cmake/glm.cmake)
include(cmake/glfw.cmake)
include(cmake/glad.cmake)
include(cmake/imgui.cmake)
include(cmake/fftw3.cmake)
include(cmake/freeimage.cmake)

add_subdirectory("src")
