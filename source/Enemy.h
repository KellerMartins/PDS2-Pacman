#include "GameEvents.h"
#include "Mapa.h"
#ifndef Enemy_H
#define Enemy_H
#include <string>

class Enemy : public GameEvents {
private:
	int isScared;
	int direcao_y;
	int direcao_x;
	int x, y;
public:
	Enemy(int x, int y);

	//virtual void calcula_direcao() = 0;
	void mover(std::vector<Enemy> enemies, Mapa m);
	int verifica_posicao(std::vector<Enemy> enemies, Mapa m);
	void morrer();
	void OnUpdate();
	void getScared();
	int get_isScared();
	int get_x();
	int get_y();
};



#endif