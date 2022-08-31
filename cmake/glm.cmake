function(init_glm)
    set(GLM_DIR ${CMAKE_SOURCE_DIR}/external/glm)
    add_library(glm INTERFACE)
    target_include_directories(glm INTERFACE ${GLM_DIR})
endfunction()

init_glm()
