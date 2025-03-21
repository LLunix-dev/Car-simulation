#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
//working?
double angle = 250;
int speed = 4;
int angle_left = 0;
int angle_right = 0;

void cleanup(SDL_Texture *imageTexture, SDL_Renderer *renderer, SDL_Window *window)
{
    SDL_DestroyTexture(imageTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(0);
}

void handleInput(SDL_Event event, SDL_Texture *imageTexture, SDL_Renderer *renderer, SDL_Window *window)
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            cleanup(imageTexture, renderer, window);
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                cleanup(imageTexture, renderer, window);
            }



            if (event.key.keysym.sym == SDLK_q) 
            {
                angle_left = 3;
            }
            if (event.key.keysym.sym == SDLK_e) 
            {
                angle_right = 3;
            }
        }
        if (event.type == SDL_KEYUP)
        {
            if (event.key.keysym.sym == SDLK_q) 
            {
                angle_left = 0;
            }
            if (event.key.keysym.sym == SDLK_e) 
            {
                angle_right = 0;
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

    SDL_Window *window = SDL_CreateWindow("Car simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *imageSurface = SDL_LoadBMP("./res/car.bmp");
    if (!imageSurface)
    {
        std::cout << "Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);

    float x = 800.0 /2;
    float y =  600.0 /2;

    SDL_Event event;

    while (true)
    {
        // Event-Handling
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        handleInput(event, imageTexture, renderer, window);
        angle += angle_right;
        angle -= angle_left;

        float radians = (M_PI * angle) / 180.0;

        float vx = speed * cos(radians);
        float vy = speed * sin(radians);


        x += vx;
        y += vy;


        drawRotatedImage(renderer, imageTexture, x, y, 200, 100, angle);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    return 0;
}