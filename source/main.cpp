#include <iostream>
#include <raylib.h>
#include "utils.h"
#include "rendering.h"
#include "gameEvents.h"
#include "mapa.h"

int main(){
    int screenWidth = 1280;
    int screenHeight = 720;

    RenderManager::Init(screenWidth, screenHeight, "PDS2 - Pacman");
    RenderManager::SetBloomDownscale(1);
    RenderManager::camera = {{ 8.0f, 8.0f, 8.0f }, { 0.0f, 1.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE};
    SetCameraMode(RenderManager::camera, CAMERA_FREE);

    Object3D smallGrid("assets/models/Plane.obj", DARKGRAY);
    Mapa m("assets/maps/mapa_exemplo.txt");
    
    while(!WindowShouldClose()){

        GameEvents::TriggerUpdate();

        if(IsKeyPressed(KEY_R))
            RenderManager::ReloadShaders();

        RenderManager::Render();
    }

    RenderManager::Quit();

    return 0;
}