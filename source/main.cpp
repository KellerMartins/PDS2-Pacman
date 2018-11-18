#include <iostream>
#include <raylib.h>
#include "utils.h"
#include "rendering.h"
#include "gameEvents.h"
#include "mapa.h"
#include "ui.h"
#include "pacman.h"
#include "enemy.h"
#include "blinky.h"
#include "inky.h"
#include "pinky.h"
#include "clyde.h"
int main(){
    int screenWidth = 1280;
    int screenHeight = 720;

    RenderManager::Init(screenWidth, screenHeight, "PDS2 - Pacman");
    RenderManager::SetBloomDownscale(1);
    RenderManager::camera = {{ 8.0f, 8.0f, 8.0f }, { 0.0f, 1.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE};
    SetCameraMode(RenderManager::camera, CAMERA_FREE);

    UI::SetFont("assets/interface/intro/IntroMetal.fnt");

    Object3D smallGrid("assets/models/map/ground.obj", DARKGRAY);
    Mapa::CarregaArquivo("assets/maps/mapa1.txt");

    Pacman pc (5,10);

    Blinky* blinky = new Blinky(1,11);
    Inky* inky = new Inky(11,11);
    Pinky* pinky = new Pinky(1,1);
    Clyde* clyde = new Clyde(11,1);

    Enemy::adiciona_inimigo(blinky);
    Enemy::adiciona_inimigo(inky);
    Enemy::adiciona_inimigo(pinky);
    Enemy::adiciona_inimigo(clyde);
    

    while(!WindowShouldClose()){

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