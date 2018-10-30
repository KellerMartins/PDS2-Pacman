#include "Enemy.h"
#include <vector>
#include <string>

using namespace std;

#define MAPA_VALIDO 0
#define MAPA_INVALIDO 1
#define POSICAO_INIMIGO 2
#define ITEM_PONTO 3
#define ITEM_FRUTA 4
#define VALOR_FRUTA 50

Enemy::Enemy(int x, int y){
	this->x = x;
	this->y = y;
	this->direcao_y = 0;
	this->direcao_x = 0;
	this->isScared = 0;
}

int Enemy::verifica_posicao(vector<Enemy> enemies, Mapa m){
	if(m.GetElementoMapa(this->x,this->y) == MAPA_INVALIDO) {
		return MAPA_INVALIDO;
	}

	return 0;
}

void Enemy::mover(std::vector<Enemy> enemies, Mapa m){
	//this->calcula_direcao(); o metodo eh virtual faz sentido chamar ele aqui?
	
	//Altera a direção efetivamente
	this->x += this->direcao_x;
	this->y += this->direcao_y;

	if(this->verifica_posicao(enemies, m)){
	this->x -= this->direcao_x;
	this->y -= this->direcao_y;
	}	
}

void Enemy::morrer(){
	this->x = 0;
	this->y = 0;
	this->isScared = 0;
}

/*void Enemy::OnUpdate(){
	this->mover();
}*/

void Enemy::getScared(){
	this->isScared = 1;
}

int Enemy::get_isScared(){
	return this->isScared;
}
int Enemy::get_x(){
	return this->x;
}
int Enemy::get_y(){
	return this->y;
}