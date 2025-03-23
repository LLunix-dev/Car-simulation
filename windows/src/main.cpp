#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>
#include <cmath>

double angle = 250;
float speed = 4.0;
float changeSpeed  = 0;
int angle_left = 0;
int angle_right = 0;
float angle_range = 3;

void cleanup(SDL_Texture *carTexture, SDL_Renderer *renderer, SDL_Window *window)
{
    SDL_DestroyTexture(carTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(0);
}

void handleInput(SDL_Event event, SDL_Texture *carTexture, SDL_Renderer *renderer, SDL_Window *window)
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || 
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        {
            cleanup(carTexture, renderer, window);
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_q: angle_left = angle_range; break;
                case SDLK_e: angle_right = angle_range; break;
                case SDLK_w: changeSpeed = 0.1; break;
                case SDLK_s: changeSpeed = -0.1; break;
                case SDLK_SPACE: changeSpeed = -1; break;
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_q: angle_left = 0; break;
                case SDLK_e: angle_right = 0; break;
                case SDLK_w:
                case SDLK_s: changeSpeed = 0; break;
            }
        }
    }
}

void drawRotatedImage(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h, double angle)
{
    SDL_Rect destinationRect = {x, y, w, h};
    SDL_Point center = {w / 2, h / 2};
    SDL_RenderCopyEx(renderer, texture, nullptr, &destinationRect, angle, &center, SDL_FLIP_NONE);
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Car simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *carSurface = SDL_LoadBMP("car.bmp");
    SDL_Surface *trackSurface = SDL_LoadBMP(" race-track.bmp");

    SDL_Texture *carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
    SDL_FreeSurface(carSurface);

    SDL_Texture *trackTexture = SDL_CreateTextureFromSurface(renderer, trackSurface);
    SDL_FreeSurface(trackSurface);

    float x = 800.0 / 2;
    float y = 600.0 / 2;

    SDL_Event event;
    float vx = 0.0f, vy = 0.0f; 
    float friction = 0.985f;
    
    while (true)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    
        handleInput(event, carTexture, renderer, window);
    
        angle += angle_right;
        angle -= angle_left;
    
        float radians = (M_PI * angle) / 180.0;
    
        if (changeSpeed != 0)
        {
            vx += changeSpeed * cos(radians);
            vy += changeSpeed * sin(radians);
        }
    
        vx *= friction;
        vy *= friction;
    
        x += vx;
        y += vy;

        if (speed > 0.0f)
        {
            angle_range = 2 + (speed / 8.0);
        }
        else
        {
            angle_range = 0;
        }
    
        drawRotatedImage(renderer, trackTexture, 0, 0, 1000, 800, 0);
        drawRotatedImage(renderer, carTexture, x, y, 200 / 1.5, 100 / 1.5, angle);
    
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    return 0;
}
