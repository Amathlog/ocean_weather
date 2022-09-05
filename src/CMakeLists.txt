file(GLOB_RECURSE EXE_SRC
    "${OceanWeather_SOURCE_DIR}/src/*.cpp"
    "${OceanWeather_SOURCE_DIR}/include/*.h"
)

add_executable(OceanWeather ${EXE_SRC})
target_link_libraries(OceanWeather glm glad glfw ImGui fftw3 FreeImage)
target_include_directories(OceanWeather PRIVATE ${CMAKE_SOURCE_DIR}/include)
set_target_properties(OceanWeather PROPERTIES FOLDER "OceanWeather")
