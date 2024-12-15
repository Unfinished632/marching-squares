#pragma once

#include <vector>

#include "engine.h"
#include "buildConfig.h"
#include "noise.h"

class Program{
    public:
    ~Program();

    void Start();
    void StartNoiseTest();

    private:
    Engine* m_engine = new Engine();
    SimplexNoise* noise = new SimplexNoise();
    const unsigned int ROW_CELL_COUNT = WINDOWSIZEX / m_engine->pixelSize + 1;
    const double SURFACE_LEVEL = 0.2;

    bool interpolate = true;

    /*
    1------2
    |      |
    |      |
    3------4
    */
    const std::vector<std::vector<bool>> EDGE_TABLE{
        {0, 0, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
        {0, 0, 1, 1},
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 0, 1},
        {0, 1, 1, 1},
        {1, 0, 0, 0},
        {1, 0, 1, 0},
        {1, 0, 0, 1},
        {1, 0, 1, 1},
        {1, 1, 0, 0},
        {1, 1, 1, 0},
        {1, 1, 0, 1},
        {1, 1, 1, 1}
    };
    /*
    @--1---@
    |      |
    3      4
    |      |
    @--2---@
    */
    const std::vector<std::vector<short>> CONTOUR_TABLE{
        {-1, -1, -1, -1},
        {1, 2, -1, -1},
        {1, 3, -1, -1},
        {2, 3, -1, -1},
        {0, 3, -1, -1},
        {0, 2, 1, 3},
        {0, 1, -1, -1},
        {0, 2, -1, -1},
        {0, 2, -1, -1},
        {0, 1, -1, -1},
        {0, 3, 1, 2},
        {0, 3, -1, -1},
        {2, 3, -1, -1},
        {1, 3, -1, -1},
        {1, 2, -1, -1},
        {-1, -1, -1, -1}
    };

    void MarchSquareAndDraw(Vec2 pos, std::vector<std::vector<bool>>& binaryImage);
    void DrawMap();
    bool IsEdgeConfig(bool squareEdges[4], int config);
    void ShowSettingsWindow();
};