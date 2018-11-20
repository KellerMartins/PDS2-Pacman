#include <iostream>
#include <sstream>
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


enum State{Menu, Game};
State currentState = Menu;

void MenuState();
void GameState();

int main(){
    int screenWidth = 1280;
    int screenHeight = 720;

    RenderManager::Init(screenWidth, screenHeight, "PDS2 - Pacman");
    RenderManager::SetBloomDownscale(1);
    RenderManager::SetCameraOffset((Vector3){0.0f, 13.5237f, 7.3416f});
    RenderManager::camera = {{ LARGURA/2, 32.0f, ALTURA/1.9 }, { LARGURA/2, 0.0f, ALTURA/2 }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE};

    UI::SetFont("assets/interface/intro/IntroShadow.fnt");

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
        #ifndef NDEBUG
        if(IsKeyPressed(KEY_R))
            RenderManager::ReloadShaders();
        #endif

        if(IsKeyDown(KEY_SPACE)){
            UI::SetFont("assets/interface/intro/IntroMetal.fnt");
            currentState = Game;
        }

        switch(currentState){
            case Menu:
                MenuState();
            break;

            case Game:
                GameState();
            break;
        }

        RenderManager::Render();
    }

    RenderManager::Quit();

    return 0;
}

void MenuState(){
    GameEvents::TriggerMenuUpdate();
    UI::DrawImage("assets/interface/title.png",0.5,0.4,0.75,0.75);
    UI::DrawTextCentered("Press space", 0.5, 0.8, 7, 0.0, { 246, 196, 2, 255 });
}

int counter = 0;
void GameState(){
    GameEvents::TriggerUpdate();
    UI::DrawImage("assets/interface/title.png",0.88,0.88,0.25,0.25);

    std::ostringstream scoreString;
    scoreString << "Score:" << counter++;
    UI::DrawTextCentered(scoreString.str(), 0.5, 0.05, 5, 0.5, RAYWHITE);
}