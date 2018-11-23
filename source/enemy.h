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
	float visualX, visualY;//TODO: substituir pelo x/y do Object3D
	int x, y;
	static std::vector<Enemy*> enemies;
	float velocidade;
	float timerMovimento;
	float timerScatter;


public:
	Enemy(int x, int y, Color color);
	virtual void set_goal(int &goal_x, int &goal_y) = 0;
	static void adiciona_inimigo(Enemy* enemy);
	static void remove_inimigos();
	static std::vector<Enemy*> &get_enemies();
	void morrer();
	void OnUpdate();
	void getScared();
	int get_isScared();
	Color get_color();
	float get_x();
	float get_y();

	//Floyd-Wasrshall

};



#endif