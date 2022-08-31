function(init_cimg)
    set(CIMG_DIR ${CMAKE_SOURCE_DIR}/external/CImg)
    add_library(CImg INTERFACE)
    target_include_directories(CImg INTERFACE ${CIMG_DIR})
endfunction()

init_cimg()
