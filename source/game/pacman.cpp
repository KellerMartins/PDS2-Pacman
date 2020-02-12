#include "pacman.h"

#include "../utils.h"

using namespace std;
#define VALOR_FRUTA 50

int Pacman::_xToGhost = 0;
int Pacman::_yToGhost = 0;
int Pacman::_dirxToGhost = 0;
int Pacman::_diryToGhost = 0;

Pacman::Pacman(int x, int y) : _modelo("assets/models/pacman/idle/player_idle_0.obj", {255,211,0,255}),
							   _andando("assets/models/pacman/walk/player_walk", 6, true),
							   _parado("assets/models/pacman/idle/player_idle", 1, true),
							   _morrendo("assets/models/pacman/die/player_die", 18, false)
{
	_andando.Preload();
	_parado.Preload();
	_morrendo.Preload();
	_modelo.position = (Vector3){(float)x, 0, (float)y};
	_x = x;
	_y = y;
	_direcaoY = 0;
	_direcaoX = 0;
	_vivo = true;
	_vidas = 3;
	_velocidade = PACMAN_SPEED_DEFAULT;
	_timerAnimacao = 0.0;
	_timerScared = 0.0;
	_scaredTime = 6;
}

bool Pacman::VerificaPosicao(){
	int ix = roundf(_x);
	int iy = roundf(_y);

	std::vector<Enemy*> &enemies = Enemy::GetEnemies();
	
	for(unsigned int i = 0; i < enemies.size(); ++i)
	{
		float enemy_x = enemies[i]->GetX();
		float enemy_y = enemies[i]->GetY();

		if(abs(enemy_x - _x)<0.5 && abs(enemy_y - _y)<0.5){
			if(enemies[i]->IsScared()){
				enemies[i]->Morrer();
				_pontuacao += 50;
			}else if(enemies[i]->GetAlive()){
				Morrer();
				Enemy::IsOver(true);
				return 0;				
			}
		}
	}	
	
	if(Mapa::GetElementoMapa(ix,iy) == ElementoMapa::Ponto) {
		_pontuacao++;
		//Altera o valor no mapa para que o ponto desapareça
		Mapa::RemoveElementoMapa(ix,iy);
	}
	
	if(Mapa::GetElementoMapa(ix,iy) == ElementoMapa::Especial){
		for (unsigned int i = 0; i < enemies.size(); ++i){
			if(enemies[i]->GetAlive()){
				enemies[i]->GetScared(true);
			}
		}
		_pontuacao+= VALOR_FRUTA;
		//Altera o valor no mapa para que o ponto desapareça
		Mapa::RemoveElementoMapa(ix,iy);
	}

	int nextx = roundf(_x + _direcaoX*0.5);
	int nexty = roundf(_y + _direcaoY*0.5);
	if(Mapa::GetElementoMapa(nextx,nexty) == ElementoMapa::Parede) {
		return true;
	}else{
		return false;
	}
	
	
}

void Pacman::CalculaDirecao(){
	//RenderManager::DrawDebugCube((Vector3){ceilf(x-1), 0.5, roundf(y)}, (Vector3){1,1,1}, WHITE);
	//RenderManager::DrawDebugCube((Vector3){floorf(x+1), 0.5, roundf(y)}, (Vector3){1,1,1}, WHITE);
	//RenderManager::DrawDebugCube((Vector3){roundf(x), 0.5, ceilf(y-1)}, (Vector3){1,1,1}, WHITE);
	//RenderManager::DrawDebugCube((Vector3){roundf(x), 0.5, floorf(y+1)}, (Vector3){1,1,1}, WHITE);

	if (IsKeyPressed(KEY_LEFT) && Mapa::GetElementoMapa(ceilf(_x-1),roundf(_y)) != ElementoMapa::Parede)
	{
		_modelo.rotationAngle = -90;
		_y = roundf(_y);
		_direcaoY = 0;
		_direcaoX = -1;
	}
	else if (IsKeyPressed(KEY_RIGHT) && Mapa::GetElementoMapa(floorf(_x+1),roundf(_y)) != ElementoMapa::Parede)
	{
		_modelo.rotationAngle = 90;
		_y = roundf(_y);
		_direcaoY = 0;
		_direcaoX = 1;
	}
	else if (IsKeyPressed(KEY_UP) && Mapa::GetElementoMapa(roundf(_x),ceilf(_y-1)) != ElementoMapa::Parede)
	{
		_modelo.rotationAngle = 180;
		_x = roundf(_x);
		_direcaoY = -1;
		_direcaoX = 0;
	}
	else if(IsKeyPressed(KEY_DOWN) && Mapa::GetElementoMapa(roundf(_x),floorf(_y+1)) != ElementoMapa::Parede)
	{
		_modelo.rotationAngle = 0;
		_x = roundf(_x);
		_direcaoY = 1;
		_direcaoX = 0;
	}
}

void Pacman::Morrer(){
	_vivo = false;
	if(_vidas>0)
		_vidas--;
	
	_timerAnimacao = 0;
}

void Pacman::Resetar(){
	_pontuacao = 0;
	_vivo = true;
	_vidas = 3;
	_velocidade = PACMAN_SPEED_DEFAULT;
	_timerAnimacao = 0.0;
}

void Pacman::OnUpdate(){
	
	_timerAnimacao += GetFrameTime() * 9 /*Frames por segundo*/;

	
	if(Enemy::IsScared()){
		 if(_timerScared < _scaredTime){
		_timerScared += GetFrameTime();
		}
		else{
			_timerScared = 0;
			Enemy::GetScared(false);
		}
	}
	
	if(_vivo){
		CalculaDirecao();
		bool colidiu = VerificaPosicao();
		if(!colidiu && (_direcaoX != 0 || _direcaoY != 0)){
			_x = fModulus(_x + _direcaoX*_velocidade*GetFrameTime(), LARGURA);
			_y = fModulus(_y + _direcaoY*_velocidade*GetFrameTime(), ALTURA);

			_andando.SetTimer(_timerAnimacao);
			_modelo.Load3DModel(_andando.GetCurrentFrame());
		}else{
			_parado.SetTimer(_timerAnimacao);
			_modelo.Load3DModel(_parado.GetCurrentFrame());
		}
	}else{
		_morrendo.SetTimer(_timerAnimacao);
		_modelo.Load3DModel(_morrendo.GetCurrentFrame());
	}
	_modelo.position = (Vector3){(float)_x, 0, (float)_y};

	RenderManager::CameraFollow(_modelo.position);

	_xToGhost = round(_x);
	_yToGhost = round(_y);
	_dirxToGhost = _direcaoX;
	_diryToGhost = _direcaoY;
}

void Pacman::OnRestart(){
	Vector2 spawn = Mapa::GetPlayerSpawn();
	_modelo.position = {spawn.x, 0, spawn.y};
	_x = spawn.x;
	_y = spawn.y;
	_direcaoY = 0;
	_direcaoX = 0;
	_vivo = true;
	_parado.SetTimer(0);
	_modelo.Load3DModel(_parado.GetCurrentFrame());
	Enemy::IsOver(false);
}

int Pacman::GetX(){
	return _xToGhost;
}
int Pacman::GetY(){
	return _yToGhost;
}

int Pacman::GetDirX(){
	return _dirxToGhost;
}
int Pacman::GetDirY(){
	return _diryToGhost;
}

bool Pacman::IsAlive(){
	return _vivo;
}

unsigned Pacman::GetPontuacao(){
	return _pontuacao;
}

unsigned Pacman::GetVidas(){
	return _vidas;
}

