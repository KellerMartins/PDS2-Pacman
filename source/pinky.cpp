#include "pinky.h"
#include "enemy.h"

Pinky::Pinky(int x, int y):
    Enemy(x,y, PINK){}

void Pinky::set_goal(int &goal_x, int &goal_y, int pac_x, int pac_y,int dir_x, int dir_y){
    if(this->vivo){
        if(!this->isScared){
           if(isScatter){
                goal_x = LARGURA -2;
                goal_y = 1;
                if(this->timerScatter > 0){
                    this->timerScatter -= 4*GetFrameTime();
                    
                }
            }
            else{
                //O goal esta duas posicoes na frente do pacman
                //direita
                if(dir_x == 1 && dir_y == 0){
                    goal_x = pac_x + 2;
                    goal_y = pac_y;
                }
                //esquerda
                else if(dir_x == -1 && dir_y == 0){
                    goal_x = pac_x - 2;
                    goal_y = pac_y;
                }
                //cima
                else if(dir_x == 0 && dir_y == -1){
                    goal_x = pac_x;
                    goal_y = pac_y - 2;
                }
                //baixo
                else if(dir_x == 0 && dir_y == 1){
                    goal_x = pac_x;
                    goal_y = pac_y + 2;
                }
            }
        }else{
            goal_x = LARGURA - 2;
            goal_y = 1;
        }
    }else{
        goal_x = 10;
        goal_y = 10;
    }
}