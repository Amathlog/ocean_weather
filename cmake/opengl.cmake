function(init_opengl)
    if(UNIX)
        set(OpenGL_GL_PREFERENCE "GLVND")
    endif(UNIX)

    find_package(OpenGL REQUIRED)
    message(${OPENGL_LIBRARIES})
endfunction()

init_opengl()
