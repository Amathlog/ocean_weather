function(init_glm)
    set(GLM_DIR ${CMAKE_SOURCE_DIR}/external/glm)
    add_library(glm INTERFACE)
    target_include_directories(glm INTERFACE ${GLM_DIR})

    if(WIN32)
        target_compile_options(glm INTERFACE /EHsc /W3 /wd4464)
    endif(WIN32)
endfunction()

init_glm()
