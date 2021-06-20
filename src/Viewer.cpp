#include <iostream>
#include <SDL2/SDL.h>

#include "Viewer.h"
#include "Fractal.h"

Viewer::Viewer(int width, int height)
    : _width(width), _height(height)
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "/n";
    }
    //Create window
    _window = SDL_CreateWindow("SDL Tutorial",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               _width,
                               _height,
                               SDL_WINDOW_SHOWN);

    if (_window == nullptr)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "/n";
    }

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    // creates a renderer to render our images
    _renderer = SDL_CreateRenderer(_window, -1, render_flags);

    if (_renderer == nullptr)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << "/n";
    }
}

Viewer::~Viewer()
{
    if (_renderer != nullptr)
    {
        // destroy renderer
        SDL_DestroyRenderer(_renderer);
    }

    if (_window != nullptr)
    {
        //Destroy window
        SDL_DestroyWindow(_window);
    }

    //Quit SDL subsystems
    SDL_Quit();
}

void Viewer::setFractal(std::unique_ptr<Fractal> fractal)
{
    this->_fractal = std::move(fractal);
}

void Viewer::run()
{
    int close = 0;

    SDL_Event event;
    // annimation loop
    while (!close)
    {
        _fractal->launch();
        render();

        SDL_RenderPresent(_renderer);

        // Events mangement
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                // handling of close button
                close = 1;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    // handling of close button
                    close = 1;
                    break;
                case SDLK_UP:
                    _fractal->moveUp();
                    break;
                case SDLK_DOWN:
                    _fractal->moveDown();
                    break;
                case SDLK_LEFT:
                    _fractal->moveLeft();
                    break;
                case SDLK_RIGHT:
                    _fractal->moveRight();
                    break;
                case SDLK_EQUALS:
                    _fractal->zoomIn();
                    break;
                case SDLK_MINUS:
                    _fractal->zoomOut();
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void Viewer::render()
{
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            unsigned int r, g, b;
            std::tie(r, g, b) = _fractal->getColour(x, y);
            SDL_SetRenderDrawColor(_renderer, r, g, b, SDL_ALPHA_OPAQUE);

            SDL_RenderDrawPoint(_renderer, x, y);
        }
    }
}
