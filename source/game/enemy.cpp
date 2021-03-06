#include "enemy.h"

#define MAPA_VALIDO 0
#define MAPA_INVALIDO 1
#define POSICAO_INIMIGO 2
#define ITEM_PONTO 3
#define ITEM_FRUTA 4
#define VALOR_FRUTA 50

std::vector<Enemy*> Enemy::_enemies;
bool Enemy::_isOver = false;
bool Enemy::_scared = false;

Enemy::Enemy(int x, int y, Color color) : _model("assets/models/ghost.obj",color){
	_spawnX = x;
	_spawnY = y;
	_x = x;
	_y = y;
	_model.position = {(float)x, 0, (float)y};
	_direcaoY = 0;
	_direcaoX = 0;
	
	_scatter = false;
	_alive = true;

	_color = color;
	_velocidade = GHOST_SPEED_DEFAULT;

	_timerMovimento = 0.0;
	_timerScatter = 0.0;
	
	_goalX = 1.0;
	_goalY = 1.0;
}

void Enemy::AdicionaInimigo(Enemy* enemy){
	_enemies.push_back(enemy);
}
void Enemy::RemoveInimigos(){
	_enemies.clear();
	
}

std::vector<Enemy*> &Enemy::GetEnemies()
{
	return _enemies;
}

void Enemy::GetScared(bool state){
	_scared = state;
}

void Enemy::Morrer(){
	_alive = false;
	_model.SetColor(WHITE);
	_model.Load3DModel("assets/models/ghost_dead.obj");
}

void Enemy::OnUpdate(){
	if(!_isOver){
		int pac_x = Pacman::GetX();
		int pac_y = Pacman::GetY();
		int dirX =  Pacman::GetDirX();
		int dirY =  Pacman::GetDirY();

		_timerScatter += GetFrameTime();
		if(_timerScatter > SCATTER_TIME) {
			_scatter = true;
		}
		if(_timerScatter <= 0){
			_scatter = false;
		}

		if(GetAlive() && IsScared()){
			_model.SetColor(PURPLE);
			_timerScatter = 0;
		}
		else if(GetAlive() && !IsScared()){
			_model.SetColor(_color);
		}
		
		int newGoalX,newGoalY;
		SetGoal(newGoalX, newGoalY, pac_x,pac_y, dirX, dirY);


		if(newGoalX >= 0 && newGoalX < LARGURA && 
		newGoalY >= 0 && newGoalY < ALTURA && 
		Mapa::GetElementoMapa(newGoalX, newGoalY) != ElementoMapa::Parede){
			_goalX = newGoalX;
			_goalY = newGoalY;
		}

		_velocidade = _scared && _alive ? GHOST_SPEED_SCARED : GHOST_SPEED_DEFAULT;
		_timerMovimento += GetFrameTime()*_velocidade;
		
		if(abs(_x-Modulus(_x+_direcaoX, LARGURA)) != LARGURA-1){
			_model.position.x = Lerp(_x, _x+_direcaoX, _timerMovimento);
		}else{
			_model.position.x = _x+_direcaoX;
			_timerMovimento = 1;
		}

		if(abs(_y-Modulus(_y+_direcaoY, ALTURA)) != ALTURA-1){
			_model.position.z = Lerp(_y, _y+_direcaoY, _timerMovimento);
		}else{
			_model.position.z = _y+_direcaoY;
			_timerMovimento = 1;
		}

		if(_timerMovimento >= 1){
			_timerMovimento = 0;
			_x = Modulus(_x+_direcaoX, LARGURA);
			_y = Modulus(_y+_direcaoY, ALTURA);
			_model.position.x = _x;
			_model.position.z = _y;
			Mapa::ObtemDirecao(_x, _y, _goalX,_goalY, _direcaoX, _direcaoY);

			if(!_alive && _x == _goalX && _y == _goalY){
				_alive = true;
				_model.SetColor(_color);
				_model.Load3DModel("assets/models/ghost.obj");
			}
		}

		float lookAngle = _direcaoX < 0 ? -90 : 
						  _direcaoX > 0 ? 90 :
						  _direcaoY < 0 ? 180 : 
						  _direcaoY > 0 ? 0 : _model.rotationAngle;
		
		_model.rotationAngle = Lerp(_model.rotationAngle, lookAngle, GetFrameTime()*5);

	}
}

void Enemy::OnRestart(){
	_model.position = {(float)_spawnX, 0, (float)_spawnY};
	_model.SetColor(_color);
	_direcaoX = 0;
	_direcaoY = 0;
	_x = _spawnX;
	_y = _spawnY;
	_scatter = false;
	_scared = false;
	_alive = true;
}

bool Enemy::IsScared(){
	return _scared;
}

void Enemy::IsOver(bool state){
	_isOver = state;
}

Color Enemy::GetColor(){
	return _scared && _alive? (Color)PURPLE : (_alive? _color : (Color)WHITE);
}

float Enemy::GetX(){
	return _model.position.x;
}

float Enemy::GetY(){
	return _model.position.z;
}

bool Enemy::GetAlive(){
	return _alive;
}