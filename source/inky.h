#ifndef INKY_H
#define INKY_H
#include "enemy.h"

class Inky : public Enemy{
public:
    Inky(int x, int y);
	//virtual void calcula_direcao() = 0;
};
#endif