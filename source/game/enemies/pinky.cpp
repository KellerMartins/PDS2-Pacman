#include "pinky.h"

Pinky::Pinky(int x, int y):
    Enemy(x,y, PINK){}

void Pinky::SetGoal(int &goalX, int &goalY, int pacX, int pacY,int dirX, int dirY){
    if(_alive){
        if(!_scared){
           if(_scatter){
                goalX = LARGURA -2;
                goalY = 1;
                if(_timerScatter > 0){
                    _timerScatter -= 4*GetFrameTime();
                    
                }
            }
            else{
                //O goal esta duas posicoes na frente do pacman
                //direita
                if(dirX == 1 && dirY == 0){
                    goalX = pacX + 2;
                    goalY = pacY;
                }
                //esquerda
                else if(dirX == -1 && dirY == 0){
                    goalX = pacX - 2;
                    goalY = pacY;
                }
                //cima
                else if(dirX == 0 && dirY == -1){
                    goalX = pacX;
                    goalY = pacY - 2;
                }
                //baixo
                else if(dirX == 0 && dirY == 1){
                    goalX = pacX;
                    goalY = pacY + 2;
                }
            }
        }else{
            goalX = LARGURA - 2;
            goalY = 1;
        }
    }else{
        goalX = 10;
        goalY = 10;
    }
}