#include <raylib.h>
#include "game/game.h"

int width = 1280;
int height = 720;
bool fullscreen = false;
RenderManager::BlurQuality quality = RenderManager::Medium;

bool OpenConfigWindow();

int main(){
    
    #ifdef NDEBUG
    if(!OpenConfigWindow())
        return 0;
    #endif
    
    Game::Init(width, height, fullscreen, quality);

    while(!WindowShouldClose()){
        #ifndef NDEBUG
        if(IsKeyPressed(KEY_R))
            RenderManager::ReloadShaders();
        #endif

        if(IsKeyDown(KEY_SPACE)){
            Game::SetState(Game::GameStart);
        }

        Game::Update();
        RenderManager::Render();
    }

    Game::Quit();

    return 0;
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
                width = 1920;
                height = 1080;
            break;
            case 2:
                width = 1366;
                height = 768;
            break;
            case 1:
                width = 1280;
                height = 720;
            break;
            case 0:
            default:
                width = 800;
                height = 450;
            break;
        }
        
        //> Renderização
        BeginDrawing();

        //Define a cor de fundo
        Color settingsCol = {255,191,0,255};
        ClearBackground({35,45,60,255});
        //Renderiza o texto
        DrawTextEx(font,"Resolução", (Vector2){5,5}, 30, 1, RAYWHITE); 
        DrawTextEx(font,FormatText("%4d x %4d",width,height),(Vector2){195, 5}, 33,2, settingsCol); 

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