#include "mapa.h"

#include "utils.h"
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

Mapa::~Mapa()
{
	DesregistraMapaRenderizavel();
}

void Mapa::CarregaArquivo(std::string arq)
{
	std::ifstream arquivo(arq);

	for(int y = 0; y < ALTURA; y++)
	{
		for(int x = LARGURA-1; x >= 0; x--)
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