#ifndef FRACTAL_H_
#define FRACTAL_H_

#include <tuple>
#include <array>
#include <vector>
#include <complex>

#include "Point.h"

class Fractal
{
public:
    Fractal(int width, int height, int num_threads);
    ~Fractal();
    // Launch threads to calculate mandelbrot set
    void launch();
    void moveUp();                                     // move center of image up
    void moveDown();                                   // move center of image down
    void moveLeft();                                   // move center of image left
    void moveRight();                                  // move center of image right
    void zoomIn();                                     // zoom in on center of image
    void zoomOut();                                    // zoom out of center of image
    std::tuple<int, int, int> getColour(int x, int y); // get coulour of the pixel
    void compute(int threadId);                        // compute the mandelbrot

private:
    int width_;
    int height_;
    int num_threads_;
    double zoom_;
    int max_iterations_ = 250;

    static constexpr float const_zoom_ = 0.9;   // constant zoom amount
    static constexpr float const_move_ = 0.005; // constant move ammount
    static constexpr int _maxIterations = 60;
    static constexpr std::complex<double> const_start_center_ = {-0.715, 0}; // initial center of image
    static constexpr double const_start_zoom_ = 0.00417455;                  // initial zoom

    std::vector<std::vector<Point>> fractal_;
    std::complex<double> center_;
    std::vector<std::thread> threads_;

    std::vector<int> calculateColour(int x, int y);
    int mandelbrotSet(int x, int y);
};

#endif // FRACTAL_H_
