#ifndef CLYDE_H
#define CLYDE_H
#include "enemy.h"

class Clyde : public Enemy{
public:
    Clyde(int x, int y);
	//virtual void calcula_direcao() = 0;
};
#endif