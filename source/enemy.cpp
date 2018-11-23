#include "enemy.h"
#include "utils.h"
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
	this->velocidade = 5;
	this->timerMovimento = 0.0;

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

bool Enemy::verifica_posicao(){
	int ix = roundf(this->x + this->direcao_x*0.5);
	int iy = roundf(this->y + this->direcao_y*0.5);

	if(Mapa::GetElementoMapa(ix,iy) == ElementoMapa::Parede) {
		return true;
	}

	return false;
}
void Enemy::morrer(){
	this->x = 0;
	this->y = 0;
	this->isScared = 0;
}

int gx = 1;
int gy = 1;

void Enemy::OnUpdate(){

	int ix = this->x;
	int iy = this->y;
	int dx = 0;
	int dy = 0;
	
	do{
		Mapa::ObtemDirecao(ix, iy, gx,gy, dx, dy);
		RenderManager::DrawDebugLine((Vector3){(float)ix, 0.5, (float)iy}, (Vector3){(float)ix+dx, 0.5, (float)iy+dy}, this->color);
		ix+=dx;
		iy+=dy;
	}while(dx != 0 || dy != 0);

	timerMovimento += GetFrameTime()*velocidade;
	visualX = Lerp(x, x+direcao_x, timerMovimento);
	visualY = Lerp(y, y+direcao_y, timerMovimento);
	if(timerMovimento > 1){
		timerMovimento = 0;
		x += direcao_x;
		y += direcao_y;
		visualX = x;
		visualY = y;
		Mapa::ObtemDirecao(x, y, gx,gy, this->direcao_x, this->direcao_y);

		//DEBUG: move um ponto de destino global toda vez que um objeto o alcança
		if(x == gx && y == gy){
			if(gx == 1){
				gx = LARGURA-2;
				gy = ALTURA-2;
			}else{
				gx = 1;
				gy = 1;
			}
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
int Enemy::get_x(){
	return this->x;
}
int Enemy::get_y(){
	return this->y;
}