function(init_glfw)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

    set(GLFW_DIR "${CMAKE_SOURCE_DIR}/external/glfw")
    add_subdirectory("${GLFW_DIR}" glfw)
    target_compile_definitions(glfw PRIVATE -DPOSIX_REQUIRED_STANDARD=199309L)
    target_compile_definitions(glfw PRIVATE -D_POSIX_C_SOURCE=POSIX_REQUIRED_STANDARD)
    target_compile_definitions(glfw PRIVATE -D_POSIX_SOURCE=POSIX_REQUIRED_STANDARD)
endfunction()

init_glfw()
