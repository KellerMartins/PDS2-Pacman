#include "pinky.h"

Pinky::Pinky(int x, int y):
    Enemy(x,y, PINK){}

void Pinky::set_goal(int &goal_x, int &goal_y){
    if(!this->isScared){
        if(this->timerScatter > 10){
            Vector2 PinkStart;
            Mapa::GetEnemySpawn(1, PinkStart);
            goal_x = PinkStart.x;
            goal_y = PinkStart.y;
        }
        else{
            //retorna 4 posicoes a frente do pacman
        }
    }else{
        Vector2 PinkStart;
        Mapa::GetEnemySpawn(1, PinkStart);
        goal_x = PinkStart.x;
        goal_y = PinkStart.y;
    }
}