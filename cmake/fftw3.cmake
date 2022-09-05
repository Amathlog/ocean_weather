function(init_fftw3)
    if(WIN32)
        option (BUILD_TESTS "Build tests" ON)
        option (ENABLE_AVX "Compile with AVX instruction set support" OFF)
        option (ENABLE_AVX2 "Compile with AVX2 instruction set support" OFF)

        set(BUILD_TESTS OFF)
        set(ENABLE_AVX OFF)
        set(ENABLE_AVX2 OFF)

        set(FFTW3_DIR "${CMAKE_SOURCE_DIR}/external/fftw3")
        add_subdirectory("${FFTW3_DIR}" fftw3)
        target_include_directories(fftw3 PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/fftw3/api>  
                                                $<INSTALL_INTERFACE:external/fftw3/api>)
    else(WIN32)
        find_package(PkgConfig REQUIRED)
        pkg_search_module(FFTW REQUIRED fftw3 IMPORTED_TARGET)
        add_library(fftw3 ALIAS PkgConfig::FFTW)
    endif(WIN32)
endfunction()

init_fftw3()
