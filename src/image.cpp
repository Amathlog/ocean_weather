#include <image.h>

#include <FreeImage.h>

#include <setjmp.h>
#include <iostream>

namespace OceanWeather {

// Boilerplate code for IO reading

/////////////////////////////////////////////////////////////////////
unsigned myReadProc(void*     buffer,
                    unsigned  size,
                    unsigned  count,
                    fi_handle handle) {
    return (unsigned)fread(buffer, size, count, (FILE*)handle);
}

unsigned myWriteProc(void*     buffer,
                     unsigned  size,
                     unsigned  count,
                     fi_handle handle) {
    return (unsigned)fwrite(buffer, size, count, (FILE*)handle);
}

int mySeekProc(fi_handle handle, long offset, int origin) {
    return fseek((FILE*)handle, offset, origin);
}

long myTellProc(fi_handle handle) { return ftell((FILE*)handle); }

/////////////////////////////////////////////////////////////////////

Image::Image(std::string_view dirname, std::string_view filename) {
    std::string filepath;
    filepath.reserve(dirname.size() + filename.size() + 1);
    filepath += dirname;
    filepath += '/';
    filepath += filename;

    FreeImageIO io{};

    io.read_proc  = myReadProc;
    io.write_proc = myWriteProc;
    io.seek_proc  = mySeekProc;
    io.tell_proc  = myTellProc;

#ifndef _WIN32
    FILE* file = fopen(filepath.c_str(), "rb");
#else
    FILE* file = nullptr;
    fopen_s(&file, filepath.c_str(), "rb");
#endif // _WIN32

    if (file) {
        // Find the buffer format
        FREE_IMAGE_FORMAT fif =
            FreeImage_GetFileTypeFromHandle(&io, (fi_handle)file, 0);

        if (fif != FIF_UNKNOWN) {
            // Load from the file handle
            FIBITMAP* dib =
                FreeImage_LoadFromHandle(fif, &io, (fi_handle)file, 0);

            if (dib) {
                m_width                = FreeImage_GetWidth(dib);
                m_height               = FreeImage_GetHeight(dib);
                unsigned numComponents = FreeImage_GetBPP(dib) >> 3;

                // For some reason, it is flipped vertically...
                // Undo it.
                FreeImage_FlipVertical(dib);

                BYTE* imageData = FreeImage_GetBits(dib);

                m_data.resize(m_width * m_height * numComponents);

                std::memcpy(m_data.data(), imageData, m_data.size());

                FreeImage_Unload(dib);
            }
        }

        fclose(file);
    }
}

} // namespace OceanWeather
