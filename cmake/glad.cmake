function(init_glad)
    set(GLAD_DIR "${CMAKE_SOURCE_DIR}/external/glad")
    add_library(glad OBJECT "${GLAD_DIR}/src/glad.c")
    target_include_directories(glad PUBLIC "${GLAD_DIR}/include")
    target_link_libraries(glad PRIVATE ${CMAKE_DL_LIBS})
endfunction()

init_glad()
