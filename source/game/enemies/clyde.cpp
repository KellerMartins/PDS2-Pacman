#include "clyde.h"

Clyde::Clyde(int x, int y):
    Enemy(x,y,YELLOW){}

void Clyde::SetGoal(int &goalX, int &goalY, int pacX, int pacY,int dirX, int dirY){
    if(_alive){
        if(!_scared){
            if(_scatter){
                goalX = LARGURA - 2;
                goalY = ALTURA - 2;
                if(_timerScatter > 0){
                    _timerScatter -= 5*GetFrameTime();
                    
                }
            }//Se a distancia entre o Clyde e o pacman for menor que 8 ele volta pro cantinho dele
            else if(abs(pacX - _x) + abs(pacY - _y) >= 8){
                goalX = pacX;
                goalY = pacY;
            }
            else{
                goalX = LARGURA - 2;
                goalY = ALTURA - 2;    
            }
        }else{
            goalX = LARGURA - 2;
            goalY = ALTURA - 2;
        }
    }else{
        goalX = 10;
        goalY = 10;
    }
}