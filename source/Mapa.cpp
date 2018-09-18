#include "Mapa.h"
#include <iostream>
#include <fstream>
/*para testar com o arquivo mapa_exemplo.txt 
 *apague a linha de legenda dentro arquivo*/
Mapa::Mapa(std::string arq)
{
	std::ifstream arquivo(arq);

	for(int i = 0; i < LINHAS; i++)
	{
		for(int j = 0; j < COLUNAS; j++)
		{
			arquivo >> this->mapa[i][j];
		}
		
	}
}
Mapa::~Mapa()
{

}