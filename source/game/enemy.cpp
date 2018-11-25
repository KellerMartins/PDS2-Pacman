#include "enemy.h"

#define MAPA_VALIDO 0
#define MAPA_INVALIDO 1
#define POSICAO_INIMIGO 2
#define ITEM_PONTO 3
#define ITEM_FRUTA 4
#define VALOR_FRUTA 50

std::vector<Enemy*> Enemy::enemies;

Enemy::Enemy(int x, int y, Color color) : model(DEFAULT_MODEL_PATH,color){
	this->spawnX = x;
	this->spawnY = y;
	this->x = x;
	this->y = y;
	this->model.position = {(float)x, 0, (float)y};
	this->direcao_y = 0;
	this->direcao_x = 0;

	this->scared = false;
	this->scatter = false;
	this->alive = true;

	this->color = color;
	this->velocidade = 2.5;

	this->timerMovimento = 0.0;
	this->timerScatter = 0.0;
	
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

void Enemy::getScared(){
	if(alive){
		scared = true;
		model.SetColor(PURPLE);
	}
}

void Enemy::morrer(){
	alive = false;
	scared = false;
	model.SetColor(WHITE);
}

void Enemy::OnUpdate(){

	int pac_x = Pacman::get_x();
	int pac_y = Pacman::get_y();
	int dirX =  Pacman::get_dirx();
	int dirY =  Pacman::get_diry();

	timerScatter += GetFrameTime();
	if(this->timerScatter > SCATTER_TIME) {
		this->scatter = true;
	}
	if(this->timerScatter <= 0){
		this->scatter = false;
	}

	int newGoalX,newGoalY;
	this->set_goal(newGoalX, newGoalY, pac_x,pac_y, dirX, dirY);
	if(newGoalX >= 0 && newGoalX < LARGURA && 
	   newGoalY >= 0 && newGoalY < ALTURA && 
	   Mapa::GetElementoMapa(newGoalX, newGoalY) != ElementoMapa::Parede){
		goalX = newGoalX;
		goalY = newGoalY;
	}

	timerMovimento += GetFrameTime()*velocidade;
	if(abs(x-Modulus(x+direcao_x, LARGURA)) != LARGURA-1){
		model.position.x = Lerp(x, x+direcao_x, timerMovimento);
	}else{
		model.position.x = x+direcao_x;
		timerMovimento = 1;
	}

	if(abs(y-Modulus(y+direcao_y, ALTURA)) != ALTURA-1){
		model.position.z = Lerp(y, y+direcao_y, timerMovimento);
	}else{
		model.position.z = y+direcao_y;
		timerMovimento = 1;
	}

	if(timerMovimento >= 1){
		timerMovimento = 0;
		x = Modulus(x+direcao_x, LARGURA);
		y = Modulus(y+direcao_y, ALTURA);
		model.position.x = x;
		model.position.z = y;
		Mapa::ObtemDirecao(x, y, this->goalX,this->goalY, this->direcao_x, this->direcao_y);

		if(!this->alive && x == this->goalX && y == this->goalY){
			this->alive = true;
			model.SetColor(color);
		}
	}


	//DEBUG: Desenho do caminho e da posição dos fantasmas
	/*int ix = this->x;
	int iy = this->y;
	int dx = 0;
	int dy = 0;

	do{
		Mapa::ObtemDirecao(ix, iy, this->goalX,this->goalY, dx, dy);
		RenderManager::DrawDebugLine((Vector3){(float)ix, 0.5, (float)iy}, (Vector3){(float)ix+dx, 0.5, (float)iy+dy}, this->color);
		ix+=dx;
		iy+=dy;
	}while(dx != 0 || dy != 0);

	RenderManager::DrawDebugCube((Vector3){(float)model.position.x, 0.5, (float)model.position.z}, (Vector3){1,1,1}, this->color);
	*/
}

void Enemy::OnRestart(){
	model.position = {(float)spawnX, 0, (float)spawnY};
	direcao_x = 0;
	direcao_y = 0;
	x = spawnX;
	y = spawnY;
	scatter = false;
	scared = false;
	alive = true;
}

bool Enemy::isScared(){
	return this->scared;
}

Color Enemy::get_color(){
	return scared? (Color)PURPLE : (alive? color : (Color)WHITE);
}

float Enemy::get_x(){
	return this->model.position.x;
}

float Enemy::get_y(){
	return this->model.position.z;
}

bool Enemy::get_alive(){
	return this->alive;
}
