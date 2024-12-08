#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "vec.h"

struct RGBA{
    public:
    RGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
};

struct Events{
    public:
    ~Events();

    std::vector<SDL_Keycode>* keys;
};

class Engine{
    public:
    Engine();
    ~Engine();

    bool programShouldClose;
    int pixelSize = 10;
    RGBA clearColor = RGBA(0, 0, 0, 255);

    void DrawLine(Vec2 start, Vec2 end, RGBA color);
    void DrawPixel(Vec2 position, RGBA color);
    void ClearBuffer();
    void Wait(Uint32 milliseconds);
    Events PollEvents();

    inline void PresentBuffer(){ SDL_RenderPresent(m_renderer); }

    private:
    SDL_Window* m_window = NULL;
    SDL_Renderer* m_renderer = NULL;

    inline void SetRenderDrawColor(RGBA color){ SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a); }
};