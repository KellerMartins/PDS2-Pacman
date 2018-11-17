#include "enemy.h"

using namespace std;

#define MAPA_VALIDO 0
#define MAPA_INVALIDO 1
#define POSICAO_INIMIGO 2
#define ITEM_PONTO 3
#define ITEM_FRUTA 4
#define VALOR_FRUTA 50

std::vector<Enemy*> Enemy::enemies(0);

Enemy::Enemy(int x, int y){
	this->x = x;
	this->y = y;
	this->direcao_y = 0;
	this->direcao_x = 0;
	this->isScared = 0;
}

void Enemy::adiciona_inimigo(Enemy* enemy){
	enemies.push_back(enemy);
}
void Enemy::remove_inimigos(){
	enemies.clear();
	
}

std::vector<Enemy*> &Enemy::get_enemies()
{
	return enemies;
}

int Enemy::verifica_posicao(vector<Enemy*> enemies){
	if(Mapa::GetElementoMapa(this->x,this->y) == MAPA_INVALIDO) {
		return MAPA_INVALIDO;
	}

	return 0;
}

void Enemy::mover(vector<Enemy*> enemies){
	//Altera a direção efetivamente
	this->x += this->direcao_x;
	this->y += this->direcao_y;

	if(this->verifica_posicao(enemies)){
	this->x -= this->direcao_x;
	this->y -= this->direcao_y;
	}	
}

void Enemy::morrer(){
	this->x = 0;
	this->y = 0;
	this->isScared = 0;
}

void Enemy::OnUpdate(){
	/*this->calcula_direcao();
	
	this->timerMovimento += GetFrameTime()*this->velocidade;
	if(timerMovimento >= 1){
		timerMovimento = 0;
		mover();
	}*/
	RenderManager::DrawDebugCube((Vector3){(float)x, 0.5, (float)y}, (Vector3){1,1,1}, RED);
}

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