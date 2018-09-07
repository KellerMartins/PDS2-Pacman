#include <iostream>
#include <raylib.h>
#include "utils.h"
#include "rendering.h"

int main(){
    int screenWidth = 1280;
    int screenHeight = 720;

    RenderManager::Init(screenWidth, screenHeight, "PDS2 - Pacman");
    RenderManager::SetBloomDownscale(1);
    RenderManager::camera = {{ 8.0f, 8.0f, 8.0f }, { 0.0f, 1.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE};

    Object3D car("assets/models/Car.obj", "assets/models/textureMask.png", (Color){0, 176, 255});
    car.scale = (Vector3){2,2,2};
    RenderManager::RegisterObjectToRender(&car);
    
    while(!WindowShouldClose()){
        car.rotationAngle = GetTime()*10;
        car.SetColor((1+sin(GetTime()*1))*127, 
                     (1+sin(GetTime()*2))*127, 
                     (1+cos(GetTime()*3))*127);

        if(IsKeyPressed(KEY_R))
            RenderManager::ReloadShaders();
        
        RenderManager::Render();
    }

    RenderManager::Quit();

    return 0;
}