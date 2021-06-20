#include <iostream>

#include "Viewer.h"

Viewer::Viewer(int width, int height, int num_threads)
    : width_(width), height_(height), num_threads_(num_threads)
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "/n";
    }
    //Create window
    this->window_ = SDL_CreateWindow("Mandelbrot Viewer",
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     this->width_,
                                     this->height_,
                                     SDL_WINDOW_SHOWN);

    if (this->window_ == nullptr)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "/n";
    }

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    // creates a renderer to render our images
    this->renderer_ = SDL_CreateRenderer(this->window_, -1, render_flags);

    if (this->renderer_ == nullptr)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << "/n";
    }
}

Viewer::~Viewer()
{
    if (this->renderer_ != nullptr)
    {
        // destroy renderer
        SDL_DestroyRenderer(renderer_);
    }

    if (this->window_ != nullptr)
    {
        //Destroy window
        SDL_DestroyWindow(this->window_);
    }

    //Quit SDL subsystems
    SDL_Quit();
}

void Viewer::run()
{
    int close = 0;
    SDL_Event event;
    // annimation loop
    this->fractal_->launch();
    while (!close)
    {
        this->render();

        SDL_RenderPresent(this->renderer_);

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
                    fractal_->moveUp();
                    break;
                case SDLK_DOWN:
                    fractal_->moveDown();
                    break;
                case SDLK_LEFT:
                    fractal_->moveLeft();
                    break;
                case SDLK_RIGHT:
                    fractal_->moveRight();
                    break;
                case SDLK_EQUALS:
                    fractal_->zoomIn();
                    break;
                case SDLK_MINUS:
                    fractal_->zoomOut();
                    break;
                default:
                    break;
                }
                this->fractal_->launch();
            }
        }
    }
}

void Viewer::setFractal(std::unique_ptr<Fractal> fractal)
{
    this->fractal_ = std::move(fractal);
}

void Viewer::render()
{
    for (int y = 0; y < this->height_; y++)
    {
        for (int x = 0; x < this->width_; x++)
        {
            unsigned int r, g, b;
            std::tie(r, g, b) = this->fractal_->getColour(x, y);
            SDL_SetRenderDrawColor(this->renderer_, r, g, b, SDL_ALPHA_OPAQUE);

            SDL_RenderDrawPoint(this->renderer_, x, y);
        }
    }
}
