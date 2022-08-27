# Include OpenGL
if(UNIX)
    set(OpenGL_GL_PREFERENCE "GLVND")
endif(UNIX)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Include GLFW and GLAD
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_definitions(-DPOSIX_REQUIRED_STANDARD=199309L)
add_definitions(-D_POSIX_C_SOURCE=POSIX_REQUIRED_STANDARD)
add_definitions(-D_POSIX_SOURCE=POSIX_REQUIRED_STANDARD)
set(GLFW_DIR "${OceanWeather_SOURCE_DIR}/external/glfw")
set(GLAD_DIR "${OceanWeather_SOURCE_DIR}/external/glad")
add_subdirectory("${GLFW_DIR}" glfw)
include_directories("${GLFW_DIR}/include")
include_directories("${GLAD_DIR}/include")

# Build ImGui
set(IMGUI_DIR "${OceanWeather_SOURCE_DIR}/external/imgui")
include_directories("${IMGUI_DIR}")
include_directories("${IMGUI_DIR}/backends")

file (GLOB IMGUI_SRC
    "${IMGUI_DIR}/*cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

add_library(ImGui STATIC ${IMGUI_SRC})
set_target_properties(ImGui PROPERTIES FOLDER "external")

# FFTW3
if(WIN32)
    option (BUILD_TESTS "Build tests" ON)
    option (ENABLE_AVX "Compile with AVX instruction set support" OFF)
    option (ENABLE_AVX2 "Compile with AVX2 instruction set support" OFF)

    set(BUILD_TESTS OFF)
    set(ENABLE_AVX ON)
    set(ENABLE_AVX2 ON)

    set(FFTW3_DIR "${OceanWeather_SOURCE_DIR}/external/fftw3")
    add_subdirectory("${FFTW3_DIR}" fftw3)
    include_directories("${FFTW3_DIR}/api")
    set(FFTW_LIB fftw3)
else(WIN32)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(FFTW REQUIRED fftw3 IMPORTED_TARGET)
    include_directories(PkgConfig::FFTW)
    link_libraries(PkgConfig::FFTW)
    set(FFTW_LIB PkgConfig::FFTW)
endif(WIN32)

include_directories(${OceanWeather_SOURCE_DIR}/include)

file(GLOB_RECURSE EXE_SRC
    "${OceanWeather_SOURCE_DIR}/src/*.cpp"
    "${OceanWeather_SOURCE_DIR}/include/*.h"
    "${GLAD_DIR}/src/glad.c"
)

add_executable(OceanWeather ${EXE_SRC})
target_link_libraries(OceanWeather ${OPENGL_LIBRARIES} glfw ${CMAKE_DL_LIBS} ImGui ${FFTW_LIB})
set_target_properties(OceanWeather PROPERTIES FOLDER "OceanWeather")
