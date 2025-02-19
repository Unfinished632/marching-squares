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

    bool interpolate = true;
    int cellSize = 10;
    Vec2 rowCellCount = {
        WINDOWSIZEX / cellSize + 1,
        WINDOWSIZEY / cellSize + 1,
    };
    float surfaceLevel = 0.2;
    Vec2 offset = {
        0,
        0
    };

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