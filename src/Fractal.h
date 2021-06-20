#ifndef FRACTAL_H_
#define FRACTAL_H_

#include <vector>
#include <complex>
#include <thread>
#include <future>
#include <mutex>
#include <memory>
#include <SDL2/SDL.h>

#include "Point.h"
#include "FractalQueue.h"

class Fractal
{
public:
    // Fractal();
    Fractal(int width, int height, int numThreads);
    ~Fractal();

    std::tuple<int, int, int> getColour(int x, int y) const { return _fractal[y][x].getColour(); }

    void launch();
    void wait();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void zoomIn();
    void zoomOut();

protected:
    std::vector<std::thread> threads;

private:
    const int _width;
    const int _height;
    const int _numThreads;

    static constexpr float _cZoom = 0.9;
    static constexpr float _cMove = 0.0005;
    static constexpr int _maxIterations = 250;
    static constexpr std::complex<float> _cStartCenter = {-0.715, 0};

    std::vector<std::vector<Point>> _fractal;
    std::complex<float> _center;
    std::mutex _mutex;

    float _zoom;
    float _divider;

    std::vector<int> calculateColour(int x, int y);
    void compute(int threadId);
    int mandelbrotSet(int x, int y);
};

#endif // FRACTAL_H_
