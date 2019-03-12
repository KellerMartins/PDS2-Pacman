# <img align="left" src="../assets/pacIconSmall.png" width="40" height="40" />PDS2 - Pacman
<img src="../assets/menu.gif" width="256" height="256" /> <img src="../assets/gameplay.gif" width="455" height="256" />

Trabalho final da disciplina Programação e Desenvolvimento de Software 2 - UFMG

## Grupo:
 - Keller Clayderman
   - Gráficos, Arte, Gameplay, AI
 - Taynara Cruz 
    - Gameplay, AI
 - Rennan Cordeiro e Breno Matos 
    - Gameplay

## Como jogar:
 - [Baixe o executável](https://github.com/KellerMartins/PDS2-Pacman/releases) ou clone o repositório e compile o jogo com o comando `make release`
 - Execute o jogo, configure suas opções gráficas e pressione Jogar
 - Inicie o jogo com espaço
   - Setas movimentam o Pacman
   - ESC fecha o jogo
 

## Compilando:
### Dependências de Compilação:
 - Linux: 
   - g++ ou clang++
   - libx11-dev
   - libgl1-mesa-dev
 - Windows
   - MinGW64 g++
     
### Comandos:
- make
  - Compila a versão de desenvolvimento do jogo
  - Neste modo, são exibidos elementos de debug no jogo, contador de FPS e mensagens de debug no terminal
  - Não possui menu de opções, com resolução travada em 800x450
     
- make release
  - Compila a versão final do jogo
  - Sem símbolos (-g) e com otimização de código ativada (-O3)
  - Possui um menu de opções
