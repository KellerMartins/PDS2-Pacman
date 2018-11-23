#include "blinky.h"
Blinky::Blinky(int x, int y):
    Enemy(x,y, RED){}

void Blinky::set_goal(int &goal_x, int &goal_y){
    if(!this->isScared){
        if(this->timerScatter > 10){
            Vector2 BlinkStart;
            Mapa::GetEnemySpawn(0, BlinkStart);
            goal_x = BlinkStart.x;
            goal_y = BlinkStart.y;
        }
        else{
            //coordenadas do pacman
        }
    }else{
        Vector2 BlinkStart;
        Mapa::GetEnemySpawn(0, BlinkStart);
        goal_x = BlinkStart.x;
        goal_y = BlinkStart.y;
    }
}