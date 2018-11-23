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
	this->color = color;
	this->direcao_y = 0;
	this->direcao_x = 0;
	this->isScared = 0;
	this->velocidade = 2.5;
	this->timerAnimacao = 0.0;
	this->itr_x = 0;//aleatorio
	this->itr_y = 0;

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
void Enemy::calcula_direcao(int goal_x, int goal_y){

	if(goal_x > this->x)
	{
		this->direcao_y = 0;
		this->direcao_x = 1;
	}
	else if(goal_x < this->x)
	{
		this->direcao_y = 0;
		this->direcao_x = -1;
	}
	else if(goal_y < this->y)
	{
		this->direcao_y = -1;
		this->direcao_x = 0;
	}
	else if(goal_y > this->y)
	{
		this->direcao_y = 1;
		this->direcao_x = 0;
	}

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

void Enemy::OnUpdate(){

	Mapa::ObtemCaminho(this->x, this->y, 1,5, this->itr_x, this->itr_y);
	int ix = 1;
	int iy = 1;
	int gx = LARGURA - 2;
	int gy = ALTURA - 2;
	
	while(ix >=0 && iy >=0){
		int tx = ix;
		int ty = iy;
		Mapa::ObtemCaminho(ix, iy, gx,gy, ix, iy);
		RenderManager::DrawDebugLine((Vector3){(float)tx, 0.5, (float)ty}, (Vector3){(float)ix, 0.5, (float)iy}, WHITE);
	}
	
	this->calcula_direcao(this->itr_x,this->itr_y);

	this->timerAnimacao += GetFrameTime() * 9/*Frames por segundo*/;
	
	
	bool colidiu = this->verifica_posicao();
	if(!colidiu){
		this->x = fModulus(this->x + this->direcao_x*this->velocidade*GetFrameTime(), LARGURA);
		this->y = fModulus(this->y + this->direcao_y*this->velocidade*GetFrameTime(), ALTURA);
		
		timerAnimacao = (int)timerAnimacao > 5/*Num frames - 1*/ ? 0 : timerAnimacao;
		//this->model.Load3DModel(walk[(int)timerAnimacao]);
	}else{
		timerAnimacao = (int)timerAnimacao > 0/*Num frames - 1*/ ? 0 : timerAnimacao;
		//this->model.Load3DModel(idle[(int)timerAnimacao]);
	}

	//this->model.position = (Vector3){(float)x, 0, (float)y};

	RenderManager::DrawDebugCube((Vector3){(float)x, 0.5, (float)y}, (Vector3){1,1,1}, this->color);
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