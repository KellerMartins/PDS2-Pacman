#include "clyde.h"

Clyde::Clyde(int x, int y):
    Enemy(x,y,YELLOW){}

void Clyde::set_goal(int &goal_x, int &goal_y, int pac_x, int pac_y,int dir_x, int dir_y){
    if(alive){
        if(!scared){
            if(scatter){
                goal_x = LARGURA - 2;
                goal_y = ALTURA - 2;
                if(this->timerScatter > 0){
                    this->timerScatter -= 4*GetFrameTime();
                    
                }
            }//Se a distancia entre o Clyde e o pacman for menor que 8 ele volta pro cantinho dele
            else if(abs(pac_x - this->x) + abs(pac_y - this->y) >= 8){
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