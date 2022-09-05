function(init_imgui)

    set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/external/imgui")

    file (GLOB IMGUI_SRC
        "${IMGUI_DIR}/*cpp"
        "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
        "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
    )

    add_library(ImGui STATIC ${IMGUI_SRC})
    target_link_libraries(ImGui glfw)
    set_target_properties(ImGui PROPERTIES FOLDER "external")
    target_include_directories(ImGui PUBLIC "${IMGUI_DIR}")
    target_include_directories(ImGui PRIVATE "${IMGUI_DIR}/backends")

    if (WIN32)
        target_compile_options(ImGui PRIVATE /W0)
    endif(WIN32)

endfunction()

init_imgui()
