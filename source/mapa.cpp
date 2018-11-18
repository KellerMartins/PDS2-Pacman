#include "mapa.h"

#include "utils.h"

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

	Mapa& mapa = GetMapaGlobal();

	for(int y = 0; y < ALTURA; y++)
	{
		for(int x = LARGURA-1; x >= 0; x--)
		{
			int elemento;
			arquivo >> elemento;
			if(!arquivo.fail())
			{
				mapa._mapa[x][y] = (ElementoMapa) elemento;
			}
			else
			{
				throw std::invalid_argument("Mapa " + arq +" contem valor invalido!");
			}
		}
	}
	mapa.DesregistraMapaRenderizavel();
	mapa.RegistraMapaRenderizavel();
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

void Mapa::OnUpdate()
{
	for(unsigned int i=0; i<_objetosMapa.size(); i++)
	{
		Object3D* objMapa = _objetosMapa.at(i);
		objMapa->SetColor((1+sin(objMapa->position.x/8 + GetTime()*1))*127, 
				  		 (1+sin(objMapa->position.z/8 + GetTime()*2))*127, 
						 (1+cos(objMapa->position.x/8 + objMapa->position.z/8 +GetTime()*3))*127);
	}
}