#include "clyde.h"

Clyde::Clyde(int x, int y):
    Enemy(x,y,YELLOW){}

void Clyde::set_goal(int &goal_x, int &goal_y){
    if(!this->isScared){
        if(this->timerScatter > 10){
            Vector2 ClydeStart;
            Mapa::GetEnemySpawn(3, ClydeStart);
            goal_x = ClydeStart.x;
            goal_y = ClydeStart.y;
        }
        else if(/*distancia entre clyde e pacman > 8 celulas*/1){
            
            //coordenadas do pacman
        }
        else{
            Vector2 ClydeStart;
            Mapa::GetEnemySpawn(3, ClydeStart);
            goal_x = ClydeStart.x;
            goal_y = ClydeStart.y;    
        }
    }else{
        Vector2 ClydeStart;
        Mapa::GetEnemySpawn(3, ClydeStart);
        goal_x = ClydeStart.x;
        goal_y = ClydeStart.y;
    }

}