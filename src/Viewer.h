#ifndef VIEWER_H_
#define VIEWER_H_

#include <mutex>
#include <SDL2/SDL.h>

#include "Fractal.h"

class Viewer
{
public:
    Viewer(int width, int height);
    ~Viewer();
    void run();
    void setFractal(std::unique_ptr<Fractal> fractal);

private:
    SDL_Window *_window;
    SDL_Renderer *_renderer;
    std::unique_ptr<Fractal> _fractal;

    void compute();
    void render();

    const int _width;
    const int _height;
};

#endif // VIEWER_H_
