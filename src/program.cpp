#include <iostream>
#include <ostream>
#include <vector>
#include <cmath>
#include <imgui.h>

#include "program.h"
#include "buildConfig.h"
#include "noise.h"
#include "utils.h"

Program::~Program(){
    delete m_engine;
}

void Program::Start(){
    m_engine->clearColor = RGBA(0, 0, 100, 255);

    while(!m_engine->programShouldClose){
        m_engine->PollEvents();

        DrawMap();
        ShowSettingsWindow();
        m_engine->PresentBuffer();

        m_engine->Wait(10);
    }
}

void Program::ShowSettingsWindow(){
    ImGui::Begin("Settings");

    ImGui::Checkbox("Linear Interpolation", &interpolate);
    ImGui::SliderInt("Square Size", &cellSize, 10, 100);
    ImGui::SliderFloat("Surface Level", &surfaceLevel, -1, 1);

    ImGui::SliderInt("X Position", &offset.x, 0, 220);
    ImGui::SliderInt("Y Position", &offset.y, 0, 220);

    rowCellCount = WINDOWSIZEX / cellSize + 1;

    ImGui::End();
}

void Program::DrawMap(){
    m_engine->ClearBuffer();

    std::vector<std::vector<bool>> binaryImage(rowCellCount, std::vector<bool>(rowCellCount, 0));

    for(int x = 0; x < rowCellCount; x++){ // 0 49
        for(int y = 0; y < rowCellCount; y++){
            binaryImage.at(x).at(y) = noise->GetNoise2D(Vec2F(x + offset.x, y + offset.y)) >= surfaceLevel;
        }
    }

    // Marching
    for(int x = 0; x < rowCellCount - 1; x++){
        for(int y = 0; y < rowCellCount - 1; y++){
            MarchSquareAndDraw(Vec2(x, y), binaryImage);
        }
    }
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

void Program::MarchSquareAndDraw(Vec2 pos, std::vector<std::vector<bool>>& binaryImage){
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

    Vec2 LINE_POINTS[4] = {
        Vec2(cellSize / 2, 1),
        Vec2(cellSize / 2, cellSize),
        Vec2(1 ,cellSize / 2),
        Vec2(cellSize ,cellSize / 2)
    };

    if(interpolate){
        const double CORNER_VALUES[4] = {
            noise->GetNoise2D(Vec2F(pos.x + offset.x, pos.y + offset.y)),
            noise->GetNoise2D(Vec2F(pos.x + 1 + offset.x, pos.y + offset.y)),
            noise->GetNoise2D(Vec2F(pos.x + offset.x, pos.y + 1 + offset.y)),
            noise->GetNoise2D(Vec2F(pos.x + 1 + offset.x, pos.y + 1 + offset.y))
        };

        const double ESTIMATED_VALUES[4] = {
            std::lerp(1, cellSize, (surfaceLevel - CORNER_VALUES[0]) / (CORNER_VALUES[1] - CORNER_VALUES[0])),
            std::lerp(1, cellSize, (surfaceLevel - CORNER_VALUES[2]) / (CORNER_VALUES[3] - CORNER_VALUES[2])),
            std::lerp(1, cellSize, (surfaceLevel - CORNER_VALUES[0]) / (CORNER_VALUES[2] - CORNER_VALUES[0])),
            std::lerp(1, cellSize, (surfaceLevel - CORNER_VALUES[1]) / (CORNER_VALUES[3] - CORNER_VALUES[1]))
        };

        LINE_POINTS[0] = Vec2(ESTIMATED_VALUES[0], 1);
        LINE_POINTS[1] = Vec2(ESTIMATED_VALUES[1], cellSize);
        LINE_POINTS[2] = Vec2(1 ,ESTIMATED_VALUES[2]);
        LINE_POINTS[3] = Vec2(cellSize ,ESTIMATED_VALUES[3]);
    }

    for(int i = 0; i < 4; i += 2){
        short contourStart = CONTOUR_TABLE.at(edgeConfig).at(i);
        short contourEnd = CONTOUR_TABLE.at(edgeConfig).at(i + 1);

        if(contourEnd == -1){
            break;;
        }

        Vec2 lineStart = LINE_POINTS[contourStart];
        Vec2 lineEnd =  LINE_POINTS[contourEnd];

        lineStart = Vec2(pos.x * cellSize + lineStart.x, pos.y * cellSize + lineStart.y);
        lineEnd = Vec2(pos.x * cellSize + lineEnd.x, pos.y * cellSize + lineEnd.y);

        m_engine->DrawLine(lineStart, lineEnd, RGBA(0, 255, 0, 255));
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