#ifndef CLYDE_H
#define CLYDE_H
#include "enemy.h"

class Clyde : public Enemy{
public:
    Clyde(int x, int y);
    virtual void set_goal(int &goal_x, int &goal_y);
};
#endif