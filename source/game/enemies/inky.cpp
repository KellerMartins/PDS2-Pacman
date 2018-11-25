#include "inky.h"

Inky::Inky(int x, int y):
    Enemy(x,y, {12,250,255,255}){}

void Inky::set_goal(int &goal_x, int &goal_y, int pac_x, int pac_y,int dir_x, int dir_y){
    if(alive){
        if(!scared){
           if(scatter){
                goal_x = 1;
                goal_y = ALTURA - 2;
                if(this->timerScatter > 0){
                    this->timerScatter -= 4*GetFrameTime();
                }
            }
            else{
                //O goal esta uma posicao na frente do pacman
                //direita
                if(dir_x == 1 && dir_y == 0){
                    goal_x = pac_x + 1;
                    goal_y = pac_y;
                }
                //esquerda
                else if(dir_x == -1 && dir_y == 0){
                    goal_x = pac_x - 1;
                    goal_y = pac_y;
                }
                //cima
                else if(dir_x == 0 && dir_y == -1){
                    goal_x = pac_x;
                    goal_y = pac_y - 1;
                }
                //baixo
                else if(dir_x == 0 && dir_y == 1){
                    goal_x = pac_x;
                    goal_y = pac_y + 1;
                }
                
            }
        }else{
            goal_x = 1;
            goal_y = ALTURA - 2;
        }
    }else{
        goal_x = 10;
        goal_y = 10;
    }
    
}