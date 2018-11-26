#include "blinky.h"

Blinky::Blinky(int x, int y):
    Enemy(x,y, RED){}

void Blinky::SetGoal(int &goalX, int &goalY, int pacX, int pacY,int dirX, int dirY){
    if(_alive){
        if(!_scared){
            //Depois de um tempo o fantasma vai para o seu canto
            if(_scatter){
                goalX = 1;
                goalY = 1;
                if(_timerScatter > 0){
                    _timerScatter -= 4*GetFrameTime();
                    
                }
            }else{
                //segue o pacman
                goalX = pacX;
                goalY = pacY;
            }
        }else{
            //Quando ele esta com medo ele tambem vai para o seu canto
            goalX = 1;
            goalY = 1;
        }
    }else{
        goalX = 10;
        goalY = 10;
    }
}
