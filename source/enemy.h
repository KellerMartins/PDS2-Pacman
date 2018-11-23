#ifndef Enemy_H
#define Enemy_H

#include <vector>
#include <raylib.h>
#include "gameEvents.h"
#include "mapa.h"
#include "utils.h"
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <list>
#include <algorithm>
#include <stdexcept>

class Enemy : public GameEvents {
protected:
	int isScared;
	int direcao_y;
	int direcao_x;
	Color color;
	float x, y;
	static std::vector<Enemy*> enemies;
	float velocidade;
	float timerAnimacao;
	float timerScatter;
	
	//Floyd-Wasrshall
	int itr_x;
	int itr_y;


public:
	Enemy(int x, int y, Color color);
	void calcula_direcao(int goal_x, int goal_y);
	virtual void set_goal(int &goal_x, int &goal_y) = 0;
	static void adiciona_inimigo(Enemy* enemy);
	static void remove_inimigos();
	static std::vector<Enemy*> &get_enemies();
	bool verifica_posicao();
	void morrer();
	void OnUpdate();
	void getScared();
	int get_isScared();
	Color get_color();
	int get_x();
	int get_y();

	//Floyd-Wasrshall

};



#endif