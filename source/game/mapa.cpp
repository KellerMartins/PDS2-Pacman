#include "mapa.h"

#include <climits>
#include "../utils.h"
#include "enemy.h"

std::string modeloElemento[] = {"",
								"assets/models/map/wall.obj",
								"assets/models/map/point.obj",
								"assets/models/map/special.obj",
							   };

Mapa& Mapa::GetMapaGlobal(){
	static Mapa mapaGlobal;
	return mapaGlobal;
}

Mapa::Mapa(){}

Mapa::~Mapa()
{
	DesregistraMapaRenderizavel();
}

void Mapa::CarregaArquivo(std::string arq)
{
	std::ifstream arquivo(arq);

	for(int y = 0; y < ALTURA; y++)
	{
		for(int x = 0; x < LARGURA; x++)
		{
			int elemento;
			arquivo >> elemento;
			if(!arquivo.fail())
			{
				if(elemento <= MAX_INDICE_ELEMENTO_MAPA){
					_mapa[x][y] = (ElementoMapa) elemento;
				}else{
					if(elemento == INDICE_PACMAN){
						_playerSpawn = (Vector2){(float)x, (float)y};
					}
					else if(elemento >= INDICE_FANTASMAS){
						_enemySpawn[elemento - INDICE_FANTASMAS] = (Vector2){(float)x, (float)y};
					}
				}
			}
			else
			{
				throw std::invalid_argument("Mapa " + arq +" contem valor invalido!");
			}
		}
	}
	DesregistraMapaRenderizavel();
	RegistraMapaRenderizavel();
	GerarCaminho();
}

void Mapa::GerarCaminho(){
	//Criando a matriz geradora de caminhos
	int** _caminhos = new int*[LARGURA*ALTURA];
	for(int i = 0; i < LARGURA*ALTURA; i++){
		_caminhos[i] = new int[LARGURA*ALTURA];
	}
	//Inicializando
	for(int i = 0; i < LARGURA*ALTURA; i++){
		for(int j = 0; j < LARGURA*ALTURA; j++){
			_caminhos[i][j] = INT_MAX/2;
			_proximo[i][j] = -1;
		}
	}

	for(int y = 0; y < ALTURA; y++)
	{
		for(int x = 0; x < LARGURA; x++)
		{
			int i = x+(y*LARGURA);
			_caminhos[i][i] = 0;
			_proximo[i][i] = i;

			//Define a distancia entre vizinhos e o atual como 1
			if(_mapa[x][y] != Parede){
				if(x+1 < LARGURA && _mapa[x+1][y]!=Parede){
					int iv = (x+1)+(y*LARGURA);
					_caminhos[i][iv] = 1;
					_caminhos[iv][i] = 1;
					_proximo[i][iv] = iv;
					_proximo[iv][i] = i;
				}else if(x == LARGURA){
					int iv = 0+(y*LARGURA);
					_caminhos[i][iv] = 1;
					_caminhos[iv][i] = 1;
					_proximo[i][iv] = iv;
					_proximo[iv][i] = i;
				}

				if(x-1 >= 0 && _mapa[x-1][y]!=Parede){
					int iv = (x-1)+(y*LARGURA);
					_caminhos[i][iv] = 1;
					_caminhos[iv][i] = 1;
					_proximo[i][iv] = iv;
					_proximo[iv][i] = i;
				}else if(x == 0){
					int iv = (LARGURA-1)+(y*LARGURA);
					_caminhos[i][iv] = 1;
					_caminhos[iv][i] = 1;
					_proximo[i][iv] = iv;
					_proximo[iv][i] = i;
				}

				if(y+1 < ALTURA && _mapa[x][y+1]!=Parede){
					int iv = x +((y+1)*LARGURA);
					_caminhos[i][iv] = 1;
					_caminhos[iv][i] = 1;
					_proximo[i][iv] = iv;
					_proximo[iv][i] = i;
				}else if(y == ALTURA){
					int iv = x+(0*LARGURA);
					_caminhos[i][iv] = 1;
					_caminhos[iv][i] = 1;
					_proximo[i][iv] = iv;
					_proximo[iv][i] = i;
				}

				if(y-1 >= 0 && _mapa[x][y-1]!=Parede){
					int iv = x +((y-1)*LARGURA);
					_caminhos[i][iv] = 1;
					_caminhos[iv][i] = 1;
					_proximo[i][iv] = iv;
					_proximo[iv][i] = i;
				}else if(y == 0){
					int iv = x+((ALTURA-1)*LARGURA);
					_caminhos[i][iv] = 1;
					_caminhos[iv][i] = 1;
					_proximo[i][iv] = iv;
					_proximo[iv][i] = i;
				}
			}			
		}
	}

	for(int k = 0; k < LARGURA*ALTURA; k++){
		for(int i = 0; i < LARGURA*ALTURA; i++){
			for(int j = 0; j < LARGURA*ALTURA; j++){
				if(_caminhos[i][j] > _caminhos[i][k] + _caminhos[k][j]){
					_caminhos[i][j] = _caminhos[i][k] + _caminhos[k][j];
					_proximo[i][j] = _proximo[i][k];
				}
			}
		}
	}
}

void Mapa::ObtemDirecao(unsigned int startX, unsigned int startY, unsigned int goalX, unsigned int goalY, int &stepX, int &stepY){
	if(startX >= LARGURA || startY >= ALTURA|| goalX >= LARGURA || goalY >=ALTURA){
		stepX = 0;
		stepY = 0;
		return;
	}

	int u = startX + (startY*LARGURA);
	int v = goalX + (goalY*LARGURA);
	Mapa &mapa = GetMapaGlobal();
	if(mapa._proximo[u][v] >= 0 && (startX != goalX || startY != goalY)){
		stepX = mapa._proximo[u][v] % LARGURA;
		stepY = (mapa._proximo[u][v] - stepX)/LARGURA;

		stepX -= startX;
		stepY -= startY;
	}
	else{
		stepX = 0;
		stepY = 0;
	}
}

ElementoMapa Mapa::GetElementoMapa(unsigned int x, unsigned int y)
{
	if(x >= LARGURA || y >=ALTURA) return ElementoMapa::Vazio;

	return GetMapaGlobal()._mapa[x][y];
}

void Mapa::RemoveElementoMapa(unsigned int x, unsigned int y)
{
	if(x >= LARGURA || y >=ALTURA) return ;

	Mapa& mapa = GetMapaGlobal();
	mapa._mapa[x][y] = ElementoMapa::Vazio;
	for(unsigned int i=0; i < mapa._objetosMapa.size(); i++)
	{
		Object3D* objMapa = mapa._objetosMapa.at(i);
		if(objMapa->position.x == x && objMapa->position.z == y)
		{
			delete objMapa;
			mapa._objetosMapa.erase(mapa._objetosMapa.begin()+i);
			break;
		}	
	}
}

void Mapa::RegistraMapaRenderizavel()
{
	for(int y = 0; y < ALTURA; y++)
	{
		for(int x = 0; x < LARGURA; x++)
		{
			int indiceElem = (int)(this->_mapa[x][y]);
			if(indiceElem < (int)(sizeof(modeloElemento)/sizeof(modeloElemento[0])) && modeloElemento[indiceElem] != "")
			{
				Object3D* parede = new Object3D(modeloElemento[indiceElem], BLUE);
				parede->position.x = x;
				parede->position.z = y;
				_objetosMapa.push_back(parede);
			}
		}
	}
}

void Mapa::DesregistraMapaRenderizavel()
{
	for(Object3D* o : _objetosMapa)
	{
		delete o;
	}
	_objetosMapa.clear();
}

Vector2 Mapa::GetPlayerSpawn(){
	return GetMapaGlobal()._playerSpawn;
}

bool Mapa::GetEnemySpawn(unsigned id, Vector2& pos){
	auto it = GetMapaGlobal()._enemySpawn.find(id);
	if(it != GetMapaGlobal()._enemySpawn.end()){
		pos = it->second;
		return true;
	}else{
		return false;
	}
}

double tempoInterp = 0;
void Mapa::OnUpdate()
{
	tempoInterp += GetFrameTime()*0.0125;
	for(unsigned int i=0; i<_objetosMapa.size(); i++)
	{
		Object3D* objMapa = _objetosMapa.at(i);

		if(Mapa::GetElementoMapa(objMapa->position.x, objMapa->position.z) == Parede){
			float val = 0;
			int luzR = 0;
			int luzG = 0;
			int luzB = 0;

			std::vector<Enemy*>& inimigos = Enemy::get_enemies(); 
			for(Enemy* e : inimigos){
				Vector3 pos = {(float)e->get_x(), 0, (float)e->get_y()};
				float distVal = Smoothstep(0.4,0.8,1.0 - Distance(objMapa->position, pos)/7.0);

				Color corInimigo = e->get_color();
				luzR += corInimigo.r*distVal;
				luzG += corInimigo.g*distVal;
				luzB += corInimigo.b*distVal;
				val += distVal;
			}

			val = Clamp(val, 0, 1);
			luzR = Clamp(luzR, 0, 255);
			luzG = Clamp(luzG, 0, 255);
			luzB = Clamp(luzB, 0, 255);
			Color luz = (Color){(unsigned char)luzR, (unsigned char)luzG, (unsigned char)luzB, 255};

			objMapa->SetColor(Lerp(objMapa->GetColor(), Lerp(BLUE, luz, val), tempoInterp));
		}
		else{
			objMapa->SetColor(Lerp(objMapa->GetColor(), { 255, 211, 0, 255 }, tempoInterp));
		}
	}
}

void Mapa::OnMenuUpdate()
{
	tempoInterp = 0;
	for(unsigned int i=0; i<_objetosMapa.size(); i++)
	{
		Object3D* objMapa = _objetosMapa.at(i);
		objMapa->SetColor((1+sin(objMapa->position.x/8 + GetTime()*1))*127, 
				  		  (1+sin(objMapa->position.z/8 + GetTime()*2))*127, 
						  (1+cos(objMapa->position.x/8 + objMapa->position.z/8 +GetTime()*3))*127);
	}
}