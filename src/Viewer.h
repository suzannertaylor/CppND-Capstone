#ifndef VIEWER_H_
#define VIEWER_H_

#include <thread>
#include <SDL2/SDL.h>

#include "Fractal.h"

class Viewer
{
public:
    Viewer(int width, int height, int num_threads);
    ~Viewer();
    void run();
    void setFractal(std::unique_ptr<Fractal> fractal);

private:
    SDL_Window *window_;
    SDL_Renderer *renderer_;
    std::unique_ptr<Fractal> fractal_;

    void render(); // render points array using SDL

    const int width_;
    const int height_;
    const int num_threads_;
};

#endif // VIEWER_H_
