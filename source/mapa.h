#ifndef MAPA_H_
#define MAPA_H_

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "graphics/object3D.h"
#include "graphics/rendering.h"
#include "gameEvents.h"

#define ALTURA 27
#define LARGURA 21

#define MAX_INDICE_ELEMENTO_MAPA ((int)ElementoMapa::Especial)
#define INDICE_PACMAN 4

enum ElementoMapa{Vazio, Parede, Ponto, Especial};

class Mapa : public GameEvents{
	private:
		ElementoMapa _mapa[LARGURA][ALTURA];
		std::vector<Object3D*> _objetosMapa;

		Vector2 _playerSpawn;
		std::vector<Vector2> _enemySpawn;
		
		void RegistraMapaRenderizavel();
		void DesregistraMapaRenderizavel();

		Mapa() {};
		
	public:
		
		~Mapa();

		void CarregaArquivo(std::string arq);

		static Mapa& GetMapaGlobal();
		static ElementoMapa GetElementoMapa(unsigned int x, unsigned int y);
		static void RemoveElementoMapa(unsigned int x, unsigned int y);
		static Vector2 GetPlayerSpawn();

		virtual void OnUpdate() override;
		virtual void OnMenuUpdate() override;
};



#endif