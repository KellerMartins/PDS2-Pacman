#include <iostream>
#include <raylib.h>
#include "utils.h"
#include "rendering.h"
#include "gameEvents.h"
#include "mapa.h"
#include "ui.h"

int main(){
    int screenWidth = 1280;
    int screenHeight = 720;

    RenderManager::Init(screenWidth, screenHeight, "PDS2 - Pacman");
    RenderManager::SetBloomDownscale(1);
    RenderManager::camera = {{ 8.0f, 8.0f, 8.0f }, { 0.0f, 1.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE};
    SetCameraMode(RenderManager::camera, CAMERA_FREE);

    UI::SetFont("assets/interface/intro/IntroMetal.fnt");

    Object3D smallGrid("assets/models/Plane.obj", DARKGRAY);
    Mapa::CarregaArquivo("assets/maps/mapa_exemplo.txt");
    Mapa::RemoveElementoMapa(0, 3);

    Object3D pacman("assets/models/pacman/walk/player_walk_0.obj", BLUE);
    pacman.position = (Vector3){3,0,10};
    std::string walk[] = {"assets/models/pacman/walk/player_walk_0.obj",
                          "assets/models/pacman/walk/player_walk_1.obj",
                          "assets/models/pacman/walk/player_walk_2.obj",
                          "assets/models/pacman/walk/player_walk_3.obj",
                          "assets/models/pacman/walk/player_walk_4.obj",
                          "assets/models/pacman/walk/player_walk_5.obj"};

    float animTime = 0;

    while(!WindowShouldClose()){
        
        pacman.Load3DModel(walk[(int)animTime]);
        animTime += GetFrameTime() * 9/*Frames por segundo*/;
        animTime = (int)animTime > 5/*Num frames*/ ? 0 : animTime;

        GameEvents::TriggerUpdate();

        if(IsKeyPressed(KEY_R))
            RenderManager::ReloadShaders();

        UI::DrawImage("assets/interface/title.png",0.88,0.88,0.25,0.25);
        UI::DrawTextCentered("Score:8000", 0.5, 0.05, 5, 0.5, RAYWHITE);

        RenderManager::Render();
    }

    RenderManager::Quit();

    return 0;
}