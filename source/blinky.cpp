#include "blinky.h"

Blinky::Blinky(int x, int y):
    Enemy(x,y, RED){}

void Blinky::set_goal(int &goal_x, int &goal_y, int pac_x, int pac_y,int dir_x, int dir_y){
    if(this->vivo){
        if(!this->isScared){
            //Depois de um tempo o fantasma vai para o seu canto
            if(isScatter){
                goal_x = 1;
                goal_y = 1;
                if(this->timerScatter > 0){
                    this->timerScatter -= 4*GetFrameTime();
                    std::cout<<this->timerScatter<<std::endl;
                }
            }else{
                goal_x = pac_x;
                goal_y = pac_y;
            }
        }else{
            //Quando ele esta com medo ele tambem vai para o seu canto
            goal_x = 1;
            goal_y = 1;
        }
    }else{
        goal_x = 10;
        goal_y = 10;
    }
}