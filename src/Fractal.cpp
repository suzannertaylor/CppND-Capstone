#include <future>
#include <vector>
#include <iostream>

#include "Fractal.h"

Fractal::Fractal(int width, int height, int num_threads)
    : width_(width), height_(height), num_threads_(num_threads)
{
    this->center_ = this->const_start_center_;
    this->zoom_ = this->const_start_zoom_;

    for (int y = 0; y < this->height_; y++)
    {
        std::vector<Point> row(this->width_);
        this->fractal_.emplace_back(row);
    }
}

Fractal::~Fractal()
{
}

void Fractal::launch()
{
    std::vector<std::future<void>> futures;
    for (int i = 0; i < this->num_threads_; ++i)
    {
        futures.emplace_back(std::async(std::launch::deferred, &Fractal::compute, this, i));
    }

    // wait for tasks to complete
    for (const std::future<void> &ftr : futures)
    {
        ftr.wait();
    }
}

void Fractal::compute(int thread_id)
{
    std::cout << "   Processing Thread ID " << thread_id << " has been sent to the queue" << std::endl;
    for (int y = thread_id; y < this->height_; y += this->num_threads_)
    {
        for (int x = 0; x < this->width_; x++)
        {
            std::vector<int> colour = this->calculateColour(x, y);
            this->fractal_[y][x].setColour(colour[0], colour[1], colour[2]);
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

int Fractal::mandelbrotSet(int x, int y)
{
    std::complex<double> c((this->center_.real() + (x - (this->width_ / 2)) * this->zoom_),
                           (this->center_.imag() + (y - (this->height_ / 2)) * this->zoom_));
    std::complex<double> z(0.0, 0.0);

    int n = 0;

    while (abs(z) < 2.0 && n < this->max_iterations_)
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

    int n = this->mandelbrotSet(x, y);

    double smooth1 = 0.0;
    if (n == 0 || n == this->max_iterations_)
    {
        return {0, 0, 0};
    }

    smooth1 = (255 * n) / this->max_iterations_;

    auto scale = [](double value, double in_min, double in_max, double out_min, double out_max)
    {
        return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    };

    r = scale(smooth1 * smooth1, 0, 255, 0, 255);
    g = scale(smooth1, 0, 255, 0, 255);
    b = scale(smooth1, 0, sqrt(255), 0, 255);

    return {r, g, b};
}

void Fractal::moveUp()
{
    this->center_.imag(this->center_.imag() + (this->const_move_ * this->const_zoom_));
}

void Fractal::moveDown()
{
    this->center_.imag(this->center_.imag() - (this->const_move_ * this->const_zoom_));
}

void Fractal::moveLeft()
{
    this->center_.real(this->center_.real() + (this->const_move_ * this->const_zoom_));
}

void Fractal::moveRight()
{
    this->center_.real(this->center_.real() - (this->const_move_ * this->const_zoom_));
}

void Fractal::zoomIn()
{
    this->zoom_ *= this->const_zoom_;
}

void Fractal::zoomOut()
{
    this->zoom_ /= this->const_zoom_;
}

std::tuple<int, int, int> Fractal::getColour(int x, int y)
{
    return this->fractal_[y][x].getColour();
}
