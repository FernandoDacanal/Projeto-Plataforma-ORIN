/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Função principal e lógica do jogo. Template base para desenvolvimento
 * de jogos em C usando Raylib (https://www.raylib.com/).
 *
 * @copyright Copyright (c) 2026
 */
//#include <stdio.h>
//#include <stdlib.h>
#include <stdbool.h>

#include "include/GameWindow.h"

extern bool mod_desenvolvedor;

int main (int argc, char* argv[])
{
	//for (int i = 1; i < argc; i++)
	//{
	//	// Ai teria que iterar por que pode ser argumento string
	//}
	if (argc > 1)
	{
		// Eu to com preguiça de fazer passar por todos
		// os argumentos, pelo menos por enquanto.
		mod_desenvolvedor = true;
	}

    GameWindow *gameWindow = createGameWindow(
        640,             // width
        360,             // height
        "Jogo de Sonic", // title
        60,              // target FPS
        false,           // antialiasing
        true,           // resizable
        false,           // full screen
        false,           // undecorated
        false,           // always on top
        false,           // invisible background
        false,           // always run
        true,            // load resources
        true             // init audio
    );

    initGameWindow( gameWindow );

    return 0;
}
