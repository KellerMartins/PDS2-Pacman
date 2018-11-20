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

int screenWidth = 1920;
int screenHeight = 1080;
bool fullscreen = false;

enum State{Menu, Game};
State currentState = Menu;

void MenuState();
void GameState();
bool OpenConfigWindow();

int main(){
    
    if(!OpenConfigWindow())
        return 0;
    

    RenderManager::Init(screenWidth, screenHeight, fullscreen, "PDS2 - Pacman");
    RenderManager::SetBloomDownscale(1);
    RenderManager::SetCameraOffset((Vector3){0.0f, 18.5237f, 7.3416f});
    RenderManager::camera = {{ LARGURA/2, 32.0f, ALTURA/1.9 }, { LARGURA/2, 0.0f, ALTURA/2 }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE};

    UI::SetFont("assets/interface/intro/IntroShadow.fnt");
    Mapa::GetMapaGlobal().CarregaArquivo("assets/maps/mapa1.txt");

    Pacman pc (Mapa::GetPlayerSpawn().x,Mapa::GetPlayerSpawn().y);

    Vector2 enemyPos;
    if(Mapa::GetEnemySpawn(0, enemyPos))
        Enemy::adiciona_inimigo(new Blinky(enemyPos.x,enemyPos.y));

    if(Mapa::GetEnemySpawn(1, enemyPos))
        Enemy::adiciona_inimigo(new Pinky(enemyPos.x,enemyPos.y));

    if(Mapa::GetEnemySpawn(2, enemyPos))
        Enemy::adiciona_inimigo(new Inky(enemyPos.x,enemyPos.y));

    if(Mapa::GetEnemySpawn(3, enemyPos))
        Enemy::adiciona_inimigo(new Clyde(enemyPos.x,enemyPos.y));
    

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

    Enemy::remove_inimigos();

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
    std::ostringstream scoreString;
    scoreString << "Score:" << counter++;
    UI::DrawTextCentered(scoreString.str(), 0.5, 0.05, 5, 0.5, RAYWHITE);
}

bool OpenConfigWindow(){ 
    InitWindow(400, 125, "Pacman - PDS2");
    
    int currResolution = 1;
    while(1){
        if(WindowShouldClose()){
            //Fecha a janela
            CloseWindow();
            return false;
        }

        //Define os retângulos dos botões da janela
        Rectangle previous = { 10, 50, 20, 30 };
        Rectangle next = { 210, 50, 20, 30 };
        Rectangle play = { 255, 45, 125, 70 };
        Rectangle fullscr = { 10, 87, 30, 27 };
        
        //Checa se clicou em um deles ou apertou a tecla correspondente ao comando
        if((CheckCollisionPointRec(GetMousePosition(), previous) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||  IsKeyPressed(KEY_LEFT))
            currResolution -= currResolution-1>=0? 1:0;
        
        if((CheckCollisionPointRec(GetMousePosition(), next) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||  IsKeyPressed(KEY_RIGHT))
            currResolution += currResolution+1<=3? 1:0;
        
        if(CheckCollisionPointRec(GetMousePosition(), fullscr) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            fullscreen = !fullscreen;
        
        if((CheckCollisionPointRec(GetMousePosition(), play) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||  IsKeyReleased(KEY_ENTER))
            break; //Termina o loop da tela de configuraçoes
        
        //Define a resolução a ser usada
        switch(currResolution){
            case 0:
                screenWidth = 1920;
                screenHeight = 1080;
            break;
            case 1:
                screenWidth = 1366;
                screenHeight = 768;
            break;
            case 2:
                screenWidth = 1280;
                screenHeight = 720;
            break;
            case 3:
            default:
                screenWidth = 800;
                screenHeight = 450;
            break;
        }
        
        //> Renderização
        BeginDrawing();
           //Define a cor de fundo
           ClearBackground(RAYWHITE);
           //Renderiza o texto
           DrawTextEx(GetFontDefault(),"Escolha a resolução da tela: ", (Vector2){7,5}, 30, 1, BLACK); 
           DrawTextEx(GetFontDefault(),FormatText("%4d x %4d",screenWidth,screenHeight),(Vector2){35, 41}, 33,2, BLACK); 
           DrawTextEx(GetFontDefault(),"Tela cheia",(Vector2){46, 85}, 33, 1, BLACK); 
           
           //Renderiza o botão de tela cheia (se ativo ou não)
            DrawCircle(fullscr.x+fullscr.width/2, fullscr.y+fullscr.height/2, fullscr.height/2, fullscreen? (Color)GREEN : (Color)RED);
           
           //Renderiza o botão de jogar
           DrawRectangleRec(play, GREEN);
           DrawText("Jogar!",273, 65, 30, WHITE); 
            
           //Renderiza as setas de resolução
           if(currResolution>0)
                DrawTextEx(GetFontDefault(),"<",(Vector2){15, 41}, 33, 2, BLACK); 
           else
               DrawTextEx(GetFontDefault(),"<",(Vector2){15, 41}, 33, 2, LIGHTGRAY); 
           
           if(currResolution<2){
               DrawTextEx(GetFontDefault(),">",(Vector2){215, 41}, 33, 2, BLACK);
           }else{
              DrawTextEx(GetFontDefault(),">",(Vector2){215, 41}, 37, 2, LIGHTGRAY);
           }
           
        EndDrawing();
    }
    //Fecha a janela de configurações
    CloseWindow();

    return true;
}