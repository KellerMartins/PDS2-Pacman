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
RenderManager::BlurQuality quality = RenderManager::Medium;

enum State{Menu, Game};
State currentState = Menu;

void MenuState();
void GameState();
bool OpenConfigWindow();

int main(){
    
    if(!OpenConfigWindow())
        return 0;
    

    RenderManager::Init(screenWidth, screenHeight, fullscreen, "Pacman Remake - PDS2");
    RenderManager::SetBloomQuality(quality);
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
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(575, 110, "Opções | Pacman Remake - PDS2");
    
    Font font = LoadFont("assets/interface/metropolis/Metropolis.fnt");
    SetTextureFilter(font.texture, FILTER_TRILINEAR);

    int resolution = 3;
    while(1){
        if(WindowShouldClose()){
            //Fecha a janela
            CloseWindow();
            return false;
        }

        //Define os retângulos dos botões da janela
        Rectangle play = { 430, 10, 135, 90 };
        Rectangle fullscr = { 175, 75, 240, 27 };
        Rectangle previousRes = { 175, 5, 20, 30 };
        Rectangle nextRes = { 400, 5, 20, 30 };
        Rectangle previousQual = { 175, 40, 20, 30 };
        Rectangle nextQual = { 400, 40, 20, 30 };

        //Checa se clicou em um deles ou apertou a tecla correspondente ao comando
        if((CheckCollisionPointRec(GetMousePosition(), previousRes) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
            resolution -= resolution-1>=0? 1:0;
        
        if((CheckCollisionPointRec(GetMousePosition(), nextRes) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
            resolution += resolution+1<=3? 1:0;

        if((CheckCollisionPointRec(GetMousePosition(), previousQual) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
            quality = (RenderManager::BlurQuality) (quality-1>=0? quality-1:quality);
        
        if((CheckCollisionPointRec(GetMousePosition(), nextQual) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
            quality = (RenderManager::BlurQuality) (quality+1<=2? quality+1:quality);
        
        if(CheckCollisionPointRec(GetMousePosition(), fullscr) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            fullscreen = !fullscreen;
        
        if((CheckCollisionPointRec(GetMousePosition(), play) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||  IsKeyReleased(KEY_ENTER))
            break; //Termina o loop da tela de configuraçoes
        
        //Define a resolução a ser usada
        switch(resolution){
            case 3:
                screenWidth = 1920;
                screenHeight = 1080;
            break;
            case 2:
                screenWidth = 1366;
                screenHeight = 768;
            break;
            case 1:
                screenWidth = 1280;
                screenHeight = 720;
            break;
            case 0:
            default:
                screenWidth = 800;
                screenHeight = 450;
            break;
        }
        
        //> Renderização
        BeginDrawing();

        //Define a cor de fundo
        Color settingsCol = {255,191,0,255};
        ClearBackground({35,45,60,255});
        //Renderiza o texto
        DrawTextEx(font,"Resolução", (Vector2){5,5}, 30, 1, RAYWHITE); 
        DrawTextEx(font,FormatText("%4d x %4d",screenWidth,screenHeight),(Vector2){195, 5}, 33,2, settingsCol); 

        const char* qualityString = quality==2 ?" Alta " : (quality==1? "Média" : "Baixa ");
        DrawTextEx(font,"Qualidade", (Vector2){5,40}, 30, 1, RAYWHITE); 
        DrawTextEx(font, qualityString, (Vector2){255, 40}, 33, 2, settingsCol); 

        DrawTextEx(font,"Tela cheia", (Vector2){5, 75}, 30, 1, RAYWHITE); 
        DrawTextEx(font, fullscreen? "   Ativado" : "Desativado", (Vector2){205, 75}, 33, 2, settingsCol); 
        
        //Renderiza o botão de jogar
        DrawRectangleRec(play, {255,191,0,255});
        DrawTextEx(font, "Jogar!",(Vector2){455, 40}, 30, 0, WHITE); 
        
        //Renderiza as setas de resolução
        DrawTextEx(font,"<",(Vector2){175, 5}, 33, 2, resolution>0? settingsCol : (Color)DARKGRAY); 
        DrawTextEx(font,">",(Vector2){398, 5}, 33, 2, resolution<3? settingsCol : (Color)DARKGRAY);

        //Renderiza as setas de qualidade
        DrawTextEx(font,"<",(Vector2){175, 40}, 33, 2, quality>0? settingsCol : (Color)DARKGRAY); 
        DrawTextEx(font,">",(Vector2){398, 40}, 33, 2, quality<2? settingsCol : (Color)DARKGRAY);
           
        EndDrawing();
    }
    //Fecha a janela de configurações
    CloseWindow();

    return true;
}