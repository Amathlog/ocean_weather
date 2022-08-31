#include <fftw3.h>
#include <window.h>

#include <iostream>

void TestFFT() {
    size_t N = 10;

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

int main(int, char**) {
    OceanWeather::Window window("Ocean Weather", 640, 480);
    if (!window) return 1;

    while (!window.ShouldClose()) {
        window.Update();
    }
    return 0;
}
