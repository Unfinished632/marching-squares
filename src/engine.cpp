#include <SDL2/SDL_render.h>
#include <iostream>
#include <vector>

#include "engine.h"
#include "macros.h"
#include "buildConfig.h"

Engine::Engine(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        std::cout << "Error initializing SDL: " << SDL_GetError() << '\n';
        PAUSE();
        throw -1;
    }

    m_window = SDL_CreateWindow("Marching Squares", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOWSIZEX, WINDOWSIZEY, SDL_WINDOW_SHOWN);

    if(!m_window){
        std::cout << "Error initializing window: " << SDL_GetError() << '\n';
        PAUSE();
        throw -1;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_SOFTWARE);

    if(!m_renderer){
        std::cout << "Error creating renderer: " << SDL_GetError() << '\n';
        PAUSE();
        throw -1;
    }
}

Engine::~Engine(){
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Engine::DrawLine(Vec2 start, Vec2 end, RGBA color){
    SetRenderDrawColor(color);
    SDL_RenderDrawLine(m_renderer, start.x, start.y, end.x, end.y);
}

void Engine::DrawPixel(Vec2 position, RGBA color){
    SetRenderDrawColor(color);
    SDL_Rect pixel = {.x = position.x * pixelSize, .y = position.y * pixelSize, .w = pixelSize, .h = pixelSize};
    SDL_RenderFillRect(m_renderer, &pixel);
}

Events Engine::PollEvents(){
    SDL_Event event;
    std::vector<SDL_Keycode>* keys = new std::vector<SDL_Keycode>;

    do{
        switch (event.type){
            case SDL_QUIT:
                programShouldClose = true;
                break;
            case SDL_KEYDOWN:
                keys->push_back(event.key.keysym.sym);
                break;
        }
    }while(SDL_PollEvent(&event) != 0);

    return Events{.keys = keys};
}

void Engine::ClearBuffer(){
    SetRenderDrawColor(clearColor);
    SDL_RenderClear(m_renderer);
}

void Engine::Wait(Uint32 milliseconds){
    SDL_Delay(milliseconds);
}

RGBA::RGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

Events::~Events(){
    delete keys;
}