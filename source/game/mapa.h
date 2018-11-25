#ifndef MAPA_H_
#define MAPA_H_

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include "../graphics/object3D.h"
#include "../graphics/rendering.h"
#include "gameEvents.h"

#define ALTURA 27
#define LARGURA 21

#define MAX_INDICE_ELEMENTO_MAPA ((int)ElementoMapa::Fruta)
#define INDICE_PACMAN 5
#define INDICE_FANTASMAS 6

enum ElementoMapa{Vazio, Parede, Ponto, Especial, Fruta};

class Mapa : public GameEvents{
	private:
		ElementoMapa _mapa[LARGURA][ALTURA];
		std::vector<Object3D*> _objetosMapa;

		Vector2 _playerSpawn;
		std::map<unsigned, Vector2> _enemySpawn;
		int _proximo[LARGURA*ALTURA][LARGURA*ALTURA];
		
		void RegistraMapaRenderizavel();
		void DesregistraMapaRenderizavel();
		void GerarCaminho();
		
	public:
		Mapa();
		~Mapa();

		void CarregaArquivo(std::string arq);

		static Mapa& GetMapaGlobal();
		static ElementoMapa GetElementoMapa(unsigned int x, unsigned int y);
		static void RemoveElementoMapa(unsigned int x, unsigned int y);
		static Vector2 GetPlayerSpawn();
		static bool GetEnemySpawn(unsigned id, Vector2& pos);

		virtual void OnUpdate() override;
		virtual void OnMenuUpdate() override;

		static void ObtemDirecao(unsigned int startX, unsigned int startY, unsigned int goalX, unsigned int goalY, int &stepX, int &stepY);

};



#endif