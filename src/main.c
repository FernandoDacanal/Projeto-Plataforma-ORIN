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
#include <string.h>
#include <stdbool.h>

#include "include/GameWindow.h"

extern bool mod_desenvolvedor;
extern int multiplayer;
extern char ipServidor[256];

int main (int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		switch (argv[i][0])
		{
			case 'd':
				mod_desenvolvedor = true;
				break;

			case 'm':
				// "m s <ip>" → servidor
				if (i + 1 < argc && argv[i + 1][0] == 's')
				{
					if (i + 2 >= argc) {
						return 1;
					}
					multiplayer = 1;
					memset(ipServidor, 0, sizeof(ipServidor));
					strncpy(ipServidor, argv[i + 2], sizeof(ipServidor) - 1);
					i += 2;
				}
				// "m <ip>" → cliente
				else
				{
					if (i + 1 >= argc) {
						return 1;
					}
					multiplayer = 2;
					memset(ipServidor, 0, sizeof(ipServidor));
					strncpy(ipServidor, argv[i + 1], sizeof(ipServidor) - 1);
					i += 1;
				}
				break;
		}
	}

    GameWindow *gameWindow = createGameWindow(
        960,             // width
        540,             // height
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
