#ifndef Enemy_H
#define Enemy_H

#include <raylib.h>
#include <vector>
#include <list>
#include <stdexcept>
#include <cmath>

#include "gameEvents.h"
#include "mapa.h"
#include "pacman.h"
#include "../utils.h"

#define SCATTER_TIME 20

class Enemy : public GameEvents {
protected:
	Object3D model;
	int spawnX;
	int spawnY;
	int x, y;
	int direcao_y;
	int direcao_x;

	bool scatter;
	bool scared;
	bool alive;
	
	float velocidade;
	Color color;
	
	static std::vector<Enemy*> enemies;
	
	float timerMovimento;
	float timerScatter;
	
	int goalX, goalY;

public:
	Enemy(int x, int y, Color color);
	virtual void set_goal(int &goal_x, int &goal_y, int pac_x, int pac_y,int dir_x, int dir_y) = 0;
	static void adiciona_inimigo(Enemy* enemy);
	static void remove_inimigos();
	static std::vector<Enemy*> &get_enemies();
	void morrer();
	void getScared();
	bool isScared();
	Color get_color();
	float get_x();
	float get_y();
	bool get_alive();

	virtual void OnUpdate();
	virtual void OnRestart();
};



#endif