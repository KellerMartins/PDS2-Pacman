#include "inky.h"

Inky::Inky(int x, int y):
    Enemy(x,y, {12,250,255,255}){}

void Inky::set_goal(int &goal_x, int &goal_y){
    if(!this->isScared){
        if(this->timerScatter > 10){
            Vector2 InkStart;
            Mapa::GetEnemySpawn(2, InkStart);
            goal_x = InkStart.x;
            goal_y = InkStart.y;
        }
        else{
            //retorna o ponto que a 2 posicoes a frente do pacman + distancia euclidiana de Blink ate esse ponto
        }
    }else{
        Vector2 InkStart;
        Mapa::GetEnemySpawn(2, InkStart);
        goal_x = InkStart.x;
        goal_y = InkStart.y;
    }
    
}