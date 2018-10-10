#include "Mapa.h"

Mapa::Mapa(std::string arq)
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
				this->_mapa[x][y] = (ElementoMapa) elemento;
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

Mapa::~Mapa()
{
	DesregistraMapaRenderizavel();
}

Object3D* Mapa::GetObjetoMapa(int indiceObjeto){
	return _objetosMapa.at(indiceObjeto);
}

int Mapa::GetNumeroObjetosMapa(){
	return _objetosMapa.size();
}

ElementoMapa Mapa::GetElementoMapa(unsigned int x, unsigned int y){
	if(x >= COLUNAS || y >=LINHAS) return ElementoMapa::Vazio;

	return _mapa[x][y];
}

void Mapa::RegistraMapaRenderizavel(){
	for(int y = 0; y < LINHAS; y++)
	{
		for(int x = 0; x < COLUNAS; x++)
		{
			switch(this->_mapa[x][y])
			{
				case Vazio:
				break;

				case Parede:
					
					Object3D* parede = new Object3D(DEFAULT_MODEL_PATH, BLUE);
					parede->position.x = x;
					parede->position.z = y;
					_objetosMapa.push_back(parede);

				break;
			}
		}
	}
}

void Mapa::DesregistraMapaRenderizavel(){
	for(Object3D* o : _objetosMapa)
	{
		delete o;
	}
	_objetosMapa.clear();
}

void Mapa::OnUpdate(){
	for(int i=0; i< GetNumeroObjetosMapa(); i++){
		Object3D* mapObj = GetObjetoMapa(i);
		mapObj->SetColor((1+sin(mapObj->position.x/8 + GetTime()*1))*127, 
				  		 (1+sin(mapObj->position.z/8 + GetTime()*2))*127, 
						 (1+cos(mapObj->position.x/8 + mapObj->position.z/8 +GetTime()*3))*127);
	}
}