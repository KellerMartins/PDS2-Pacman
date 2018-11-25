#ifndef INKY_H
#define INKY_H
#include "../enemy.h"

class Inky : public Enemy{
public:
    Inky(int x, int y);
    virtual void set_goal(int &goal_x, int &goal_y, int pac_x, int pac_y,int dir_x, int dir_y);
};
#endif