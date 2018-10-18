#include "Mapa.h"

std::string modeloElemento[] = {"",
								DEFAULT_MODEL_PATH,
								"",
								"",
							   };

Mapa Mapa::_mapaGlobal;

Mapa::~Mapa()
{
	DesregistraMapaRenderizavel();
}

void Mapa::CarregaArquivo(std::string arq)
{
	std::ifstream arquivo(arq);

	for(int y = 0; y < LINHAS; y++)
	{
		for(int x = COLUNAS-1; x >= 0; x--)
		{
			int elemento;
			arquivo >> elemento;
			if(!arquivo.fail())
			{
				_mapaGlobal._mapa[x][y] = (ElementoMapa) elemento;
			}
			else
			{
				throw std::invalid_argument("Mapa " + arq +" contem valor invalido!");
			}
		}
	}
	_mapaGlobal.DesregistraMapaRenderizavel();
	_mapaGlobal.RegistraMapaRenderizavel();
}

ElementoMapa Mapa::GetElementoMapa(unsigned int x, unsigned int y)
{
	if(x >= COLUNAS || y >=LINHAS) return ElementoMapa::Vazio;

	return _mapaGlobal._mapa[x][y];
}

void Mapa::RemoveElementoMapa(unsigned int x, unsigned int y)
{
	if(x >= COLUNAS || y >=LINHAS) return ;

	_mapaGlobal._mapa[x][y] = ElementoMapa::Vazio;
	for(unsigned int i=0; i<_mapaGlobal._objetosMapa.size(); i++)
	{
		Object3D* objMapa = _mapaGlobal._objetosMapa.at(i);
		if(objMapa->position.x == x && objMapa->position.z == y)
		{
			delete objMapa;
			_mapaGlobal._objetosMapa.erase(_mapaGlobal._objetosMapa.begin()+i);
			break;
		}	
	}
}

void Mapa::RegistraMapaRenderizavel()
{
	for(int y = 0; y < LINHAS; y++)
	{
		for(int x = 0; x < COLUNAS; x++)
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