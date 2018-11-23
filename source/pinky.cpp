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
                    std::cout<<this->timerScatter<<std::endl;
                }
            }
            else{
                //retorna 4 posicoes a frente do pacman
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