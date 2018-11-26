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
	Object3D _model;
	int _spawnX;
	int _spawnY;
	int _x, _y;
	int _direcaoY;
	int _direcaoX;

	bool _scatter;
	static bool _scared;
	bool _alive;
	static bool _isOver;
	
	float _velocidade;
	Color _color;
	
	static std::vector<Enemy*> _enemies;
	
	float _timerMovimento;
	float _timerScatter;
	
	int _goalX, _goalY;

public:
	Enemy(int _x, int _y, Color _color);
	virtual void SetGoal(int &goalX, int &goalY, int pacX, int pacY,int dirX, int dirY) = 0;
	static void AdicionaInimigo(Enemy* _enemy);
	static void RemoveInimigos();
	static void IsOver(bool state);
	static std::vector<Enemy*> &GetEnemies();
	void Morrer();
	static void GetScared(bool state);
	static bool IsScared();
	Color GetColor();
	float GetX();
	float GetY();
	bool GetAlive();

	virtual void OnUpdate();
	virtual void OnRestart();
};



#endif