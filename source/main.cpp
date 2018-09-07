#include <iostream>
#include <raylib.h>
#include "utils.h"
#include "rendering.h"

int main(){
    int screenWidth = 1280;
    int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "PDS2 - Pacman");
    SetTargetFPS(0);

    RenderManager r;
    r.SetResolution(screenWidth, screenHeight);
    r.SetBloomDownscale(1);
    r.camera = {{ 8.0f, 8.0f, 8.0f }, { 0.0f, 1.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE};

    Object3D car("assets/models/Car.obj", "assets/models/textureMask.png", (Color){0, 176, 255});
    car.scale = (Vector3){2,2,2};

    r.RegisterObjectToRender(&car);

    while(!WindowShouldClose()){
        car.rotationAngle = GetTime()*10;
        car.SetColor((1+sin(GetTime()*1))*127, 
                     (1+sin(GetTime()*2))*127, 
                     (1+cos(GetTime()*3))*127);

        if(IsKeyPressed(KEY_R))
            r.ReloadShaders();
        
        r.Render();
    }

    return 0;
}