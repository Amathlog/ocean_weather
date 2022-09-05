function(init_libjpeg)
    if(WIN32)
        set(JPEG_DIR "${CMAKE_SOURCE_DIR}/external/jpeg")

        file (GLOB JPEG_SRC
            "${JPEG_DIR}/*c"
        )

        add_library(jpeg STATIC ${JPEG_SRC})
        set_target_properties(jpeg PROPERTIES FOLDER "external")
        target_include_directories(jpeg PUBLIC "${JPEG_DIR}")
    else(WIN32)
        find_package(JPEG REQUIRED)
    endif(WIN32)
endfunction()

init_libjpeg()
