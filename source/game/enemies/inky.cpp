#include "inky.h"

Inky::Inky(int x, int y):
    Enemy(x,y, {12,250,255,255}){}

void Inky::SetGoal(int &goalX, int &goalY, int pacX, int pacY,int dirX, int dirY){
    if(_alive){
        if(!_scared){
           if(_scatter){
                goalX = 1;
                goalY = ALTURA - 2;
                if(_timerScatter > 0){
                    _timerScatter -= 5*GetFrameTime();
                }
            }
            else{
                //O goal esta uma posicao na frente do pacman
                //direita
                if(dirX == 1 && dirY == 0){
                    goalX = pacX + 3;
                    goalY = pacY;
                }
                //esquerda
                else if(dirX == -1 && dirY == 0){
                    goalX = pacX - 3;
                    goalY = pacY;
                }
                //cima
                else if(dirX == 0 && dirY == -1){
                    goalX = pacX;
                    goalY = pacY - 3;
                }
                //baixo
                else if(dirX == 0 && dirY == 1){
                    goalX = pacX;
                    goalY = pacY + 3;
                }
                
            }
        }else{
            goalX = 1;
            goalY = ALTURA - 2;
        }
    }else{
        goalX = 10;
        goalY = 10;
    }
    
}