#include <iostream>
#include <complex>
#include <thread>
#include <vector>
#include <tuple>
#include <algorithm>
#include <chrono>
#include <mutex>

#include "Fractal.h"
#include "Point.h"
#include "FractalQueue.h"

Fractal::Fractal(int width, int height, int numThreads)
    : _width(width), _height(height), _numThreads(numThreads)
{
    _center = _cStartCenter;
    _zoom = 0.00417455;

    for (int y = 0; y < _height; y++)
    {
        std::vector<Point> row(_width);
        _fractal.emplace_back(row);
    }
}

Fractal::~Fractal()
{
}

void Fractal::launch()
{
    FractalQueue q("Fractal queue", _numThreads);
    for (int i = 0; i < _numThreads; i++)
    {
        q.dispatch([i, this]()
                   { this->compute(i); });
    }
}

void Fractal::compute(int threadId)
{
    //std::cout << "   Processing Thread ID " << threadId << " has been sent to the queue" << std::endl;
    for (int y = threadId; y < _height; y += _numThreads)
    {
        for (int x = 0; x < _width; x++)
        {
            std::vector<int> colour = calculateColour(x, y);
            _fractal[y][x].setColour(colour[0], colour[1], colour[2]);
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

int Fractal::mandelbrotSet(int x, int y)
{
    std::complex<double> c((_center.real() + (x - (_width / 2)) * _zoom),
                           (_center.imag() + (y - (_height / 2)) * _zoom));
    std::complex<double> z(0.0, 0.0);

    int n = 0;

    while (abs(z) < 2.0 && n < _maxIterations)
    {
        z = (z * z) + c;
        ++n;
    }
    return n;
}

std::vector<int> Fractal::calculateColour(int x, int y)
{
    int r = 0;
    int g = 0;
    int b = 0;

    int n = mandelbrotSet(x, y);

    double smooth1 = 0.0;
    if (n == 0 || n == _maxIterations)
    {
        return {0, 0, 0};
    }

    smooth1 = (255 * n) / _maxIterations;

    auto scale = [](double value, double in_min, double in_max, double out_min, double out_max)
    {
        return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    };

    b = scale(smooth1 * smooth1, 0, 255, 0, 255);
    g = scale(smooth1, 0, 255, 0, 255);
    r = scale(smooth1, 0, sqrt(255), 0, 255);

    return {r, g, b};
}

void Fractal::moveUp()
{
    _center.imag(_center.imag() + (_cMove * _cZoom));
}

void Fractal::moveDown()
{
    _center.imag(_center.imag() - (_cMove * _cZoom));
}

void Fractal::moveLeft()
{
    _center.real(_center.real() + (_cMove * _cZoom));
}

void Fractal::moveRight()
{
    _center.real(_center.real() - (_cMove * _cZoom));
}

void Fractal::zoomIn()
{
    _zoom *= _cZoom;
}

void Fractal::zoomOut()
{
    _zoom /= _cZoom;
}
