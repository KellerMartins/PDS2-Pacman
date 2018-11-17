#ifndef Enemy_H
#define Enemy_H

#include <vector>
#include "gameEvents.h"
#include "mapa.h"

class Enemy : public GameEvents {
protected:
	int isScared;
	int direcao_y;
	int direcao_x;
	int x, y;
	static std::vector<Enemy*> enemies;

public:
	Enemy(int x, int y);
	//virtual void calcula_direcao() = 0;
	static void adiciona_inimigo(Enemy* enemy);
	static void remove_inimigos();
	static std::vector<Enemy*> &get_enemies();
	void mover(std::vector<Enemy*> enemies);
	int verifica_posicao(std::vector<Enemy*> enemies);
	void morrer();
	void OnUpdate();
	void getScared();
	int get_isScared();
	int get_x();
	int get_y();
};



#endif