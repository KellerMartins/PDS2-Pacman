#include "enemy.h"
#include "utils.h"
#include <chrono>
using namespace std;

#define MAPA_VALIDO 0
#define MAPA_INVALIDO 1
#define POSICAO_INIMIGO 2
#define ITEM_PONTO 3
#define ITEM_FRUTA 4
#define VALOR_FRUTA 50

std::vector<Enemy*> Enemy::enemies(0);

Enemy::Enemy(int x, int y, Color color){
	this->x = x;
	this->y = y;
	this->visualX = x;
	this->visualY = y;
	this->color = color;
	this->direcao_y = 0;
	this->direcao_x = 0;
	this->isScared = 0;
	this->velocidade = 2.5;
	this->timerMovimento = 0.0;
	this->timerScatter = 0.0;
	this->scatter = 20;
	this->isScatter = false;
	this->vivo = true;
	this->goalX = 1.0;
	this->goalY = 1.0;


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

void Enemy::morrer(){
	this->vivo = false;
	this->isScared = 0;
}

void Enemy::OnUpdate(){

	int pac_x = Pacman::get_x();
	int pac_y = Pacman::get_y();
	int dirX =  Pacman::get_dirx();
	int dirY =  Pacman::get_diry();

	timerScatter += GetFrameTime();
	if(this->timerScatter > this->scatter) {
		this->isScatter = true;
	}
	if(this->timerScatter <= 0){
		this->isScatter = false;
	}
	int ix = this->x;
	int iy = this->y;
	int dx = 0;
	int dy = 0;
	//set_goal(int this->goalX, int this->goalY);
	
	do{
		Mapa::ObtemDirecao(ix, iy, this->goalX,this->goalY, dx, dy);
		RenderManager::DrawDebugLine((Vector3){(float)ix, 0.5, (float)iy}, (Vector3){(float)ix+dx, 0.5, (float)iy+dy}, this->color);
		ix+=dx;
		iy+=dy;
	}while(dx != 0 || dy != 0);

	this->set_goal(this->goalX, this->goalY, pac_x,pac_y, dirX, dirY);
	timerMovimento += GetFrameTime()*velocidade;
	visualX = Lerp(x, x+direcao_x, timerMovimento);
	visualY = Lerp(y, y+direcao_y, timerMovimento);
	if(timerMovimento > 1){
		timerMovimento = 0;
		x += direcao_x;
		y += direcao_y;
		visualX = x;
		visualY = y;
		Mapa::ObtemDirecao(x, y, this->goalX,this->goalY, this->direcao_x, this->direcao_y);

		//DEBUG: move um ponto de destino global toda vez que um objeto o alcança
		if(!this->vivo && x == this->goalX && y == this->goalY){
			this->vivo = true;
		}
	}

	RenderManager::DrawDebugCube((Vector3){(float)visualX, 0.5, (float)visualY}, (Vector3){1,1,1}, this->color);
}

void Enemy::getScared(){
	this->isScared = 1;
}

int Enemy::get_isScared(){
	return this->isScared;
}
Color Enemy::get_color(){
	return this->color;
}
float Enemy::get_x(){
	return this->visualX;
}
float Enemy::get_y(){
	return this->visualY;
}
bool Enemy::get_vivo(){
	return this->vivo;
}
