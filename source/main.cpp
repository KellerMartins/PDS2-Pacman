#include <iostream>
#include <raylib.h>
#include "utils.h"
#include "graphics/rendering.h"
#include "graphics/ui.h"
#include "gameEvents.h"
#include "mapa.h"

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
    RenderManager::SetCameraOffset((Vector3){0.0f, 13.5237f, 7.3416f});
    RenderManager::camera = {{ LARGURA/2, 32.0f, ALTURA/1.9 }, { LARGURA/2, 0.0f, ALTURA/2 }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE};
    //SetCameraMode(RenderManager::camera, CAMERA_FREE);

    UI::SetFont("assets/interface/intro/IntroMetal.fnt");

    //Object3D smallGrid("assets/models/map/ground.obj", DARKGRAY);
    Mapa::GetMapaGlobal().CarregaArquivo("assets/maps/mapa1.txt");

    Pacman pc (Mapa::GetPlayerSpawn().x,Mapa::GetPlayerSpawn().y);

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