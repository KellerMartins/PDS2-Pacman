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
    double introFadeTimer = 0;

    bool hasLoaded = false;
    State currentState;
    void(*stateUpdateFunction)() = nullptr;

    void Init(int screenWidth, int screenHeight, bool fullscreen, RenderManager::BlurQuality quality){
        RenderManager::Init(screenWidth, screenHeight, fullscreen, "Pacman Remake - PDS2");
        RenderManager::SetBloomQuality(quality);
        RenderManager::SetCameraOffset((Vector3){0.0f, 18.5237f, 7.3416f});

        Mapa::CarregaArquivo("assets/maps/mapa1.txt");

        pacman = new Pacman(Mapa::GetPlayerSpawn().x,Mapa::GetPlayerSpawn().y);

        Vector2 enemyPos;
        if(Mapa::GetEnemySpawn(0, enemyPos))
            Enemy::AdicionaInimigo(new Blinky(enemyPos.x,enemyPos.y));

        if(Mapa::GetEnemySpawn(1, enemyPos))
            Enemy::AdicionaInimigo(new Pinky(enemyPos.x,enemyPos.y));

        if(Mapa::GetEnemySpawn(2, enemyPos))
            Enemy::AdicionaInimigo(new Inky(enemyPos.x,enemyPos.y));

        if(Mapa::GetEnemySpawn(3, enemyPos))
            Enemy::AdicionaInimigo(new Clyde(enemyPos.x,enemyPos.y));

        SetState(Menu);
    }

    void Quit(){
        delete pacman;
        Enemy::RemoveInimigos();
        RenderManager::Quit();
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

        #if defined(NDEBUG) && !defined(EMSCRIPTEN)
        if(1.0-introFadeTimer > 0){
            UI::DrawRect(0,0,1,1,Lerp(RAYWHITE, {0, 0, 0, 0}, introFadeTimer));
            introFadeTimer+=2*Clamp(GetFrameTime(),0,0.016);
        }
        #endif

        if(IsKeyDown(KEY_SPACE)){
            pacman->Resetar();
            GameEvents::TriggerRestart();
            SetState(Game::GameStart);
        }
    }

    
    void GameStartStateInit(){
        UI::SetFont("assets/interface/intro/IntroMetal.fnt");
    }
    void GameStartStateUpdate(){
        if(stateTimer < 2){
            UI::DrawTextCentered("START!", 0.5, 0.4, 7, 0.0, WHITE);
            GameEvents::TriggerMenuUpdate();

            Vector2 pacSpawn = Mapa::GetPlayerSpawn();
            RenderManager::CameraFollow({pacSpawn.x, 0, pacSpawn.y}, 3);
        }else{
            SetState(GameInProgress);
        }
    }

    double restartTimer = 0;
    void GameInProgressStateInit(){
        restartTimer = -1;
    }
    void GameInProgressStateUpdate(){
        GameEvents::TriggerUpdate();
        std::ostringstream scoreString;
        scoreString << "Score:" << pacman->GetPontuacao();
        UI::DrawTextCentered(scoreString.str(), 0.5, 0.05, 5, 0.5, RAYWHITE);
        for(unsigned i=0; i<pacman->GetVidas(); i++)
            UI::DrawImage("assets/interface/pacman_icon.png", 0.05 + i*0.05, 0.9, 0.1, 0.1);

        if(!pacman->IsAlive()){
            if(restartTimer < 0)
                restartTimer = 0;

            restartTimer += GetFrameTime();

            if(restartTimer >= 3){
                if(pacman->GetVidas() == 0){
                    SetState(GameOver);
                }else{
                    GameEvents::TriggerRestart();
                    SetState(GameStart);
                }
            }
        }
        else if(Mapa::GetPontosRestantes() == 0){
            SetState(LevelCleared);
        }
    }

    void LevelClearedStateInit(){
        
    }

    void LevelClearedStateUpdate(){
        if(stateTimer < 4){
            std::ostringstream scoreString;
            scoreString << "Score:" << pacman->GetPontuacao();
            UI::DrawTextCentered(scoreString.str(), 0.5, 0.05, 5, 0.5, RAYWHITE);
            UI::DrawTextCentered("Level Cleared!", 0.5, 0.5, 15, 0.0, WHITE);
            GameEvents::TriggerMenuUpdate();
        }else{
            Mapa::Recarrega();
            GameEvents::TriggerRestart();
            SetState(GameStart);
        }
    }


    void GameOverStateInit(){

    }
    void GameOverStateUpdate(){
        if(stateTimer < 4){
            std::ostringstream scoreString;
            scoreString << "Score:" << pacman->GetPontuacao();
            UI::DrawTextCentered(scoreString.str(), 0.5, 0.65, 8, 0.5, RAYWHITE);
            UI::DrawTextCentered("Game Over!", 0.5, 0.5, 15, 0.0, PURPLE);
            GameEvents::TriggerMenuUpdate();

            RenderManager::camera.position = Lerp(RenderManager::camera.position, 
                                                  {LARGURA/2, 32.0f, ALTURA/1.9}, 
                                                  stateTimer/32);
            RenderManager::camera.target = Lerp(RenderManager::camera.target,
                                                {LARGURA/2, 0.0f, ALTURA/2}, 
                                                stateTimer/32);
        }else{
            Mapa::CarregaArquivo("assets/maps/mapa1.txt");
            GameEvents::TriggerRestart();
            SetState(Menu);
        }
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
                GameInProgressStateInit();
                stateUpdateFunction = GameInProgressStateUpdate;
            break;

             case LevelCleared:
                LevelClearedStateInit();
                stateUpdateFunction = LevelClearedStateUpdate;
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