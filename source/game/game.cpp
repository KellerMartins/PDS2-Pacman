#include "game.h"

#include <iostream>
#include <sstream>
#include <raylib.h>
#include "../graphics/ui.h"
#include "../utils.h"

#include "mapa.h"
#include "pacman.h"
#include "enemy.h"
#include "enemies/blinky.h"
#include "enemies/inky.h"
#include "enemies/pinky.h"
#include "enemies/clyde.h"


namespace Game{

    Pacman* pacman = nullptr;

    double stateTimer = 0;

    bool hasLoaded = false;
    State currentState;
    void(*stateUpdateFunction)() = nullptr;

    void Init(int screenWidth, int screenHeight, bool fullscreen, RenderManager::BlurQuality quality){
        RenderManager::Init(screenWidth, screenHeight, fullscreen, "Pacman Remake - PDS2");
        RenderManager::SetBloomQuality(quality);
        RenderManager::SetCameraOffset((Vector3){0.0f, 18.5237f, 7.3416f});

        Mapa::GetMapaGlobal().CarregaArquivo("assets/maps/mapa1.txt");

        pacman = new Pacman(Mapa::GetPlayerSpawn().x,Mapa::GetPlayerSpawn().y);

        Vector2 enemyPos;
        if(Mapa::GetEnemySpawn(0, enemyPos))
            Enemy::adiciona_inimigo(new Blinky(enemyPos.x,enemyPos.y));

        if(Mapa::GetEnemySpawn(1, enemyPos))
            Enemy::adiciona_inimigo(new Pinky(enemyPos.x,enemyPos.y));

        if(Mapa::GetEnemySpawn(2, enemyPos))
            Enemy::adiciona_inimigo(new Inky(enemyPos.x,enemyPos.y));

        if(Mapa::GetEnemySpawn(3, enemyPos))
            Enemy::adiciona_inimigo(new Clyde(enemyPos.x,enemyPos.y));

        SetState(Menu);
    }

    void Quit(){
        delete pacman;
        Enemy::remove_inimigos();
        RenderManager::Quit();
    }

    void ShowIntro(){

    }

    void MenuStateInit(){
        RenderManager::camera.position = { LARGURA/2, 32.0f, ALTURA/1.9 };
        RenderManager::camera.target = { LARGURA/2, 0.0f, ALTURA/2 };
        UI::SetFont("assets/interface/intro/IntroShadow.fnt");
    }
    void MenuStateUpdate(){
        GameEvents::TriggerMenuUpdate();
        UI::DrawImage("assets/interface/title.png",0.5,0.4,0.75,0.75);
        UI::DrawTextCentered("Press space", 0.5, 0.8, 7, 0.0, { 246, 196, 2, 255 });
    }

    
    void GameStartStateInit(){
        UI::SetFont("assets/interface/intro/IntroMetal.fnt");
    }
    void GameStartStateUpdate(){
        if(stateTimer < 1){
            UI::DrawTextCentered("START!", 0.5, 0.5, 7, 0.0, WHITE);
            GameEvents::TriggerMenuUpdate();
        }else{
            SetState(GameInProgress);
        }
    }


    void GameInProgressStateUpdate(){
        GameEvents::TriggerUpdate();
        std::ostringstream scoreString;
        scoreString << "Score:" << (int)stateTimer;
        UI::DrawTextCentered(scoreString.str(), 0.5, 0.05, 5, 0.5, RAYWHITE);
    }


    void GameOverStateInit(){

    }
    void GameOverStateUpdate(){

    }

    void SetState(State state){
        stateTimer = 0;
        switch(state){
            case Intro:
            break;

            case Menu:
                MenuStateInit();
                stateUpdateFunction = MenuStateUpdate;
            break;

            case GameStart:
                GameStartStateInit();    
                stateUpdateFunction = GameStartStateUpdate;
            break;

            case GameInProgress:
                stateUpdateFunction = GameInProgressStateUpdate;
            break;

            case GameOver:
                GameOverStateInit();
                stateUpdateFunction = GameOverStateUpdate;
            break;

        }
    }

    void Update(){
        if(stateUpdateFunction != nullptr){
            stateUpdateFunction();
        }
        stateTimer += GetFrameTime();
    }
}