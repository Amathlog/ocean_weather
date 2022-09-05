#include <fftw3.h>
#include <window.h>
#include <FreeImage.h>

#include <iostream>

void TestFFT() {
    int N = 10;

    fftw_complex *in, *out;
    fftw_plan     p;

    in  = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
    p   = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_execute(p); /* repeat as needed */

    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);
}

// Boilerplate code for error handling
/**
 *   FreeImage error handler
 *   @param fif Format / Plugin responsible for the error
 *   @param message Error message
 */
void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char* message) {
    std::cout << std::endl << "*** ";
    if (fif != FIF_UNKNOWN) {
        std::cout << FreeImage_GetFormatFromFIF(fif) << " Format" << std::endl;
    }
    std::cout << message << " ***" << std::endl;
}

int main(int, char**) {
    using namespace OceanWeather;

    FreeImage_Initialise();

    FreeImage_SetOutputMessage(FreeImageErrorHandler);

    std::cout << "FreeImage version: " << FreeImage_GetVersion() << std::endl;

    Window window("Ocean Weather", 640, 480);
    if (!window) return 1;

    while (!window.ShouldClose()) {
        window.Update();
    }

    FreeImage_DeInitialise();
    return 0;
}
