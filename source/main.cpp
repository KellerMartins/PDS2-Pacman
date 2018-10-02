#include <iostream>
#include <raylib.h>
#include "utils.h"
#include "rendering.h"
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

        for(int i=0; i< m.GetNumeroObjetosMapa(); i++){
            Object3D* mapObj = m.GetObjetoMapa(i);
            mapObj->SetColor((1+sin(mapObj->position.x/8 + GetTime()*1))*127, 
                             (1+sin(mapObj->position.z/8 + GetTime()*2))*127, 
                             (1+cos(mapObj->position.x/8 + mapObj->position.z/8 +GetTime()*3))*127);
        }

        if(IsKeyPressed(KEY_R))
            RenderManager::ReloadShaders();

        RenderManager::Render();
    }

    RenderManager::Quit();

    return 0;
}