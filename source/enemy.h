#ifndef Enemy_H
#define Enemy_H

#include <vector>
#include <raylib.h>
#include "gameEvents.h"
#include "mapa.h"
#include <time.h>
#include <cmath>
#include <cstdlib>
#include <list>
#include <algorithm>

typedef struct ElementoBusca {
	int i;
	int j;
	float f;
	float h;
	float g;
}ElementoBusca;

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
	
	//Astar
	ElementoBusca grid[LARGURA][ALTURA];
	int itr_x;
	int itr_y;


public:
	Enemy(int x, int y, Color color);
	void calcula_direcao(int goal_x, int goal_y);
	//virtual void set_goal() = 0; -> maquina de estados!!
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

	//Astar
	void reset_grid();
	std::list<ElementoBusca> calcula_vizinhos(ElementoBusca elem);
	void Astar(int start_x, int start_y, int goal_x, int goal_y);

};



#endif