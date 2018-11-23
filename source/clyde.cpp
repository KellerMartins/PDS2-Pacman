#include "clyde.h"

Clyde::Clyde(int x, int y):
    Enemy(x,y,YELLOW){}

void Clyde::set_goal(int &goal_x, int &goal_y, int pac_x, int pac_y,int dir_x, int dir_y){
    if(this->vivo){
        if(!this->isScared){
            if(isScatter){
                goal_x = LARGURA - 2;
                goal_y = ALTURA - 2;
                if(this->timerScatter > 0){
                    this->timerScatter -= 4*GetFrameTime();
                    std::cout<<this->timerScatter<<std::endl;
                }
            }
            else if(/*distancia entre clyde e pacman > 8 celulas*/1){
                
                goal_x = pac_x;
                goal_y = pac_y;
            }
            else{
                goal_x = LARGURA - 2;
                goal_y = ALTURA - 2;    
            }
        }else{
            goal_x = LARGURA - 2;
            goal_y = ALTURA - 2;
        }
    }else{
        goal_x = 10;
        goal_y = 10;
    }
}