#include <iostream>
#include <thread>

#include "Viewer.h"
#include "Fractal.h"

int main()
{
    size_t num_threads = std::thread::hardware_concurrency();
    unsigned int width = 1000;
    unsigned int height = 1000;

    Viewer viewer(width, height, num_threads);

    auto fractal{std::unique_ptr<Fractal>()};
    fractal = std::make_unique<Fractal>(width, height, num_threads);
    viewer.setFractal(std::move(fractal));
    viewer.run();
    return 0;
}
