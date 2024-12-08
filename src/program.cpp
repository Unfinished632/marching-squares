#include <iostream>
#include <ostream>
#include <vector>

#include "program.h"
#include "buildConfig.h"
#include "noise.h"
#include "utils.h"

Program::~Program(){
    delete m_engine;
}

void Program::Start(){
    DrawMap();

    while(!m_engine->programShouldClose){
        m_engine->PollEvents();

        m_engine->Wait(10);
    }
}

void Program::DrawMap(){
    m_engine->ClearBuffer();

    SimplexNoise* noise = new SimplexNoise();

    std::vector<std::vector<double>> noiseTable(ROW_CELL_COUNT, std::vector<double>(ROW_CELL_COUNT, 0));
    std::vector<std::vector<bool>> binaryImage(ROW_CELL_COUNT, std::vector<bool>(ROW_CELL_COUNT, 0));

    const double SURFACE_LEVEL = 0.3;

    for(int x = 0; x < ROW_CELL_COUNT; x++){ // 0 49
        for(int y = 0; y < ROW_CELL_COUNT; y++){
            noiseTable.at(x).at(y) = noise->GetNoise2D(Vec2F(x, y));
            binaryImage.at(x).at(y) = noiseTable.at(x).at(y) >= SURFACE_LEVEL;
        }
    }

    // Marching
    for(int x = 0; x < ROW_CELL_COUNT - 1; x++){
        for(int y = 0; y < ROW_CELL_COUNT - 1; y++){
            MarchSquareAndDraw(Vec2(x, y), binaryImage, noise->GetNoise2D(Vec2F(x, y)));
        }
    }

    m_engine->PresentBuffer();
}

void Program::StartNoiseTest(){
    SimplexNoise* noise = new SimplexNoise();

    int cameraOffset = 0;

    while(!m_engine->programShouldClose){
        m_engine->PollEvents();
        m_engine->ClearBuffer();

        for(int x = 0; x < WINDOWSIZEX / m_engine->pixelSize; x++){
            for(int y = 0; y < WINDOWSIZEY / m_engine->pixelSize; y++){
                unsigned int pixelColor = static_cast<unsigned int>(std::floor(Map(noise->GetNoise2D(Vec2F(x + cameraOffset, y)), -1.0, 1.0, 0, 255)) + 0.5);
                m_engine->DrawPixel(Vec2(x, y), RGBA(pixelColor, pixelColor, pixelColor, 255));
            }
        }

        cameraOffset++;

        m_engine->PresentBuffer();

        m_engine->Wait(100);
    }
}

void Program::MarchSquareAndDraw(Vec2 pos, std::vector<std::vector<bool>>& binaryImage, double noiseValue){
    bool squareEdges[4] = {
        binaryImage.at(pos.x).at(pos.y),
        binaryImage.at(pos.x + 1).at(pos.y),
        binaryImage.at(pos.x).at(pos.y + 1),
        binaryImage.at(pos.x + 1).at(pos.y + 1)
    };

    short edgeConfig = -1;

    for(int i = 0; i < 16; i++){
        if(IsEdgeConfig(squareEdges, i)){
            edgeConfig = i;
            break;
        }
    }

    if(edgeConfig == -1){
        std::cout << "Couldn't find edge :(\n";
        return;
    }

    const Vec2 LINE_POINTS[4] = {
        Vec2(Map(noiseValue, -1, 1, 1, 5), 1),
        Vec2(Map(noiseValue, -1, 1, 1, 5), 10),
        Vec2(1, Map(noiseValue, -1, 1, 1, 5)),
        Vec2(10, Map(noiseValue, -1, 1, 1, 5))
    };

    for(int i = 0; i < 4; i += 2){
        short contourStart = CONTOUR_TABLE.at(edgeConfig).at(i);
        short contourEnd = CONTOUR_TABLE.at(edgeConfig).at(i + 1);

        if(contourEnd == -1){
            break;;
        }

        std::cout << "Position: " << pos.x << ", " << pos.y << '\n';

        Vec2 lineStart = LINE_POINTS[contourStart];
        Vec2 lineEnd =  LINE_POINTS[contourEnd];

        std::cout << "SquareEdges: " << squareEdges[0] << ", " << squareEdges[1] << ", " << squareEdges[2] << ", " << squareEdges[3] << '\n';
        std::cout << "edgeConfig: " << edgeConfig << '\n';
        std::cout << "RelativeLineStart: " << lineStart.x << ", " << lineStart.y << '\n';
        std::cout << "RelativeLineEnd: " << lineEnd.x << ", " << lineEnd.y << '\n';

        lineStart = Vec2(pos.x * m_engine->pixelSize + lineStart.x, pos.y * m_engine->pixelSize + lineStart.y);
        lineEnd = Vec2(pos.x * m_engine->pixelSize + lineEnd.x, pos.y * m_engine->pixelSize + lineEnd.y);

        std::cout << "LineStart: " << lineStart.x << ", " << lineStart.y << '\n';
        std::cout << "LineEnd: " << lineEnd.x << ", " << lineEnd.y << '\n';
        std::cout << "-------------------------" << std::endl;

        m_engine->DrawLine(lineStart, lineEnd, RGBA(255, 255, 255, 255));
    }
}

void Program::StartLineTest(){
    while(!m_engine->programShouldClose){
        m_engine->PollEvents();
        m_engine->ClearBuffer();

        m_engine->DrawLine(Vec2(245, 490), Vec2(49, 245), RGBA(255, 255, 255, 255));

        m_engine->PresentBuffer();

        m_engine->Wait(100);
    }
}

bool Program::IsEdgeConfig(bool squareEdges[4], int config){
    for(int j = 0; j < 4; j++){
        if(squareEdges[j] != EDGE_TABLE.at(config).at(j)){
            return false;
        }
    }

    return true;
}