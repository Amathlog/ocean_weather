#include <image.h>

#include <jpeglib.h>

#include <setjmp.h>
#include <fstream>
#include <iostream>

namespace OceanWeather {

// Boilerplate code for Jpeg error handling
struct JpegErrorManager {
    jpeg_error_mgr pub;
    jmp_buf        setjmpBuffer;
};

using JpegErrorManagerPtr = JpegErrorManager*;

void JpegErrorExit(j_common_ptr cinfo) {
    /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    JpegErrorManagerPtr myerr = (JpegErrorManagerPtr)cinfo->err;

    /* Always display the message. */
    /* We could postpone this until after returning, if we chose. */
    (*cinfo->err->output_message)(cinfo);

    /* Return control to the setjmp point */
    longjmp(myerr->setjmpBuffer, 1);
}

Image::Image(std::string_view dirname, std::string_view filename) {
    std::string filepath;
    filepath.reserve(dirname.size() + filename.size() + 1);
    filepath += dirname;
    filepath += '/';
    filepath += filename;

    struct jpeg_decompress_struct cinfo;
    struct JpegErrorManager       jerr;

    FILE* file = fopen(filepath.c_str(), "rb");

    if (file) {
        cinfo.err           = ::jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = JpegErrorExit;
        if (setjmp(jerr.setjmpBuffer)) {
            ::jpeg_destroy_decompress(&cinfo);
            fclose(file);
            return;
        }

        ::jpeg_create_decompress(&cinfo);
        ::jpeg_stdio_src(&cinfo, file);
        ::jpeg_read_header(&cinfo, TRUE);
        ::jpeg_start_decompress(&cinfo);

        m_height = cinfo.output_height;
        m_width  = cinfo.output_width;

        int row_stride = m_width * cinfo.output_components;
        m_data.reserve(m_height * row_stride);

        while (cinfo.output_scanline < m_height) {
            uint8_t* p = m_data.data() + cinfo.output_scanline * row_stride;
            ::jpeg_read_scanlines(&cinfo, &p, 1);
        }

        ::jpeg_finish_decompress(&cinfo);
        ::jpeg_destroy_decompress(&cinfo);

        fclose(file);
    }
}

} // namespace OceanWeather
