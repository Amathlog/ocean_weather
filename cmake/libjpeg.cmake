function(init_libjpeg)
    if(WIN32)
        # TODO
    else(WIN32)
        find_package(JPEG REQUIRED)
    endif(WIN32)
endfunction()

init_libjpeg()
