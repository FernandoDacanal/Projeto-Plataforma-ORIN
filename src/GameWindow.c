/**
 * @file GameWindow.c
 * @author Prof. Dr. David Buzatto
 * @brief GameWindow implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <stdio.h>
#include <math.h>

#include "include/GameWindow.h"
#include "include/GameWorld.h"
#include "include/ResourceManager.h"
#include "include/raylib/raylib.h"

#include <netdb.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "extras/multiplayer_include.h"

static void enviar_estado(int sock, Jogador *j) {
    char buf[128];
    snprintf(buf, sizeof(buf), "POS %.2f %.2f %d %d",
             j->ret.x, j->ret.y,
             j->olhandoParaDireita ? 1 : -1,
             (int) j->estado);
    mp_enviar(sock, buf);
}

GameWorld *gw_global = NULL;

/**
 * @brief Creates a dinamically allocated GameWindow struct instance.
 */
GameWindow *createGameWindow( 
        int width, 
        int height, 
        const char *title, 
        int targetFPS,
        bool antialiasing, 
        bool resizable, 
        bool fullScreen,
        bool undecorated, 
        bool alwaysOnTop, 
        bool invisibleBackground, 
        bool alwaysRun, 
        bool loadResources, 
        bool initAudio ) {

    GameWindow *gameWindow = (GameWindow*) malloc( sizeof( GameWindow ) );

    gameWindow->width = width;
    gameWindow->height = height;
    gameWindow->title = title;
    gameWindow->targetFPS = targetFPS;
    gameWindow->antialiasing = antialiasing;
    gameWindow->resizable = resizable;
    gameWindow->fullScreen = fullScreen;
    gameWindow->undecorated = undecorated;
    gameWindow->alwaysOnTop = alwaysOnTop;
    gameWindow->invisibleBackground = invisibleBackground;
    gameWindow->alwaysRun = alwaysRun;
    gameWindow->loadResources = loadResources;
    gameWindow->initAudio = initAudio;
    gameWindow->gw = NULL;
    gameWindow->initialized = false;

    return gameWindow;

}

/**
 * @brief Initializes the Window, starts the game loop and, when it
 * finishes, the window will be finished and destroyed too.
 */
void initGameWindow( GameWindow *gameWindow ) {

    if ( !gameWindow->initialized ) {

        gameWindow->initialized = true;

        if ( gameWindow->antialiasing ) {
            SetConfigFlags( FLAG_MSAA_4X_HINT );
        }

        if ( gameWindow->resizable ) {
            SetConfigFlags( FLAG_WINDOW_RESIZABLE );
        }

        if ( gameWindow->fullScreen ) {
            SetConfigFlags( FLAG_FULLSCREEN_MODE );
        }

        if ( gameWindow->undecorated ) {
            SetConfigFlags( FLAG_WINDOW_UNDECORATED );
        }

        if ( gameWindow->alwaysOnTop ) {
            SetConfigFlags( FLAG_WINDOW_TOPMOST );
        }

        if ( gameWindow->invisibleBackground ) {
            SetConfigFlags( FLAG_WINDOW_TRANSPARENT );
        }

        if ( gameWindow->alwaysRun ) {
            SetConfigFlags( FLAG_WINDOW_ALWAYS_RUN );
        }
		
		
		static int servsock, cliente;
		struct sockaddr_in info;
		
		if (multiplayer == 1)
		{
			info.sin_family = AF_INET;
			info.sin_addr.s_addr = INADDR_ANY;
			info.sin_port = htons(PORTA);
			
			
			if ((servsock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
				exit(1);
			
			{
				int opt = 1;
				setsockopt(servsock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
			}

			if ((bind(servsock, (struct sockaddr*)&info, sizeof(info))) < 0)
				exit(2);

			if (listen(servsock, 10) < 0)
				exit(3);
			
ACEITAR:
			if ((cliente = accept(servsock, 0, 0)) < 0)
				goto ACEITAR;
			
			char buffer[256] = {0};
			int n = recv(cliente, buffer, sizeof(buffer) - 1, 0);
			if (n > 0) {
				buffer[n] = '\0';
				if (strncmp(buffer, "sim", 3) == 0)
					send(cliente, "ok", 2, 0);
			}
			
			fcntl(cliente, F_SETFL, fcntl(cliente, F_GETFL, 0) | O_NONBLOCK);
		}
		else if (multiplayer == 2)
		{
			struct addrinfo dica, *resultado, *p;
			char porta_str[16];

			snprintf(porta_str, sizeof(porta_str), "%d", PORTA);

			memset(&dica, 0, sizeof(dica));
			dica.ai_family   = AF_INET;
			dica.ai_socktype = SOCK_STREAM;
			dica.ai_flags    = 0;

			int r = getaddrinfo(ipServidor, porta_str, &dica, &resultado);
			if (r != 0) {
				exit(1);
			}

			cliente = -1;
			for (p = resultado; p != NULL; p = p->ai_next) {
				cliente = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
				if (cliente < 0)
					continue;

				if (connect(cliente, p->ai_addr, p->ai_addrlen) == 0)
					break;

				close(cliente);
				cliente = -1;
			}

			freeaddrinfo(resultado);

			if (cliente < 0)
				exit(2);

			send(cliente, "sim", 3, 0);

			char buffer[256] = {0};
			int n = recv(cliente, buffer, sizeof(buffer) - 1, 0);
			if (n <= 0 || strncmp(buffer, "ok", 2) != 0)
				exit(3);
			
			fcntl(cliente, F_SETFL, fcntl(cliente, F_GETFL, 0) | O_NONBLOCK);
		}

        InitWindow( gameWindow->width, gameWindow->height, gameWindow->title );
        SetWindowMonitor(0);

        gameWindow->renderTarget = LoadRenderTexture(
            LARGURA_VIRTUAL,
            ALTURA_VIRTUAL
        );

        SetTextureFilter(
            gameWindow->renderTarget.texture,
            TEXTURE_FILTER_POINT
        );

        if ( gameWindow->initAudio ) {
            InitAudioDevice();
        }

        SetTargetFPS( gameWindow->targetFPS );    

        if ( gameWindow->loadResources ) {
            loadResourcesResourceManager();
        }

        gameWindow->gw = createGameWorld();

        // game loop
        while ( !WindowShouldClose() ) {
            // O delta time é limitado a 1/30s para evitar que frames muito
            // longos (ex.: lentidão na inicialização) causem deslocamentos
            // grandes demais, fazendo personagens atravessarem obstáculos
            // (tunneling).
            float delta = GetFrameTime();
            if ( delta > 1.0f / 30.0f ) {
                delta = 1.0f / 30.0f;
            }
            

            updateGameWorld( gameWindow->gw, delta );

            BeginTextureMode( gameWindow->renderTarget );
            drawGameWorld( gameWindow->gw );
            EndTextureMode();

            BeginDrawing();
            ClearBackground( BLACK );
            float escala = fminf(
                (float)GetScreenWidth() / LARGURA_VIRTUAL,
                (float)GetScreenHeight() / ALTURA_VIRTUAL
            );
            if (escala < 1) escala = 1;
            int larguraFinal = LARGURA_VIRTUAL * escala;
            int alturaFinal = ALTURA_VIRTUAL * escala;
            int offsetX = (GetScreenWidth() - larguraFinal) / 2;
            int offsetY = (GetScreenHeight() - alturaFinal) / 2;

            DrawTexturePro(
                gameWindow->renderTarget.texture,
                (Rectangle){0, 0, LARGURA_VIRTUAL, -ALTURA_VIRTUAL},
                (Rectangle){offsetX, offsetY, larguraFinal, alturaFinal},
                (Vector2){0,0},
                0,
                WHITE
            );
            if(IsKeyPressed(KEY_F11)){
                ToggleFullscreen();
            }
            EndDrawing();
			
			// a cada N frames, manda o estado
			static int contador_rede = 0;
			if (multiplayer != 0 && cliente >= 0) {
				if (mp_receber(cliente) < 0) {
					// outro lado caiu
					close(cliente);
					cliente = -1;
					multiplayer = 0;
				}

				if (++contador_rede >= 3) {          // ~20 Hz a 60 FPS
					enviar_estado(cliente, gameWindow->gw->jogador);
					contador_rede = 0;
				}
			}
		}

		if (servsock >= 0) close(servsock);
		if (cliente  >= 0) close(cliente);

        if ( gameWindow->loadResources ) {
            unloadResourcesResourceManager();
        }

        bool initAudio = gameWindow->initAudio;

        destroyGameWindow( gameWindow );

        if ( initAudio ) {
            CloseAudioDevice();
        }

        CloseWindow();
    }
}

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWindow( GameWindow *gameWindow ) {
    if ( gameWindow != NULL ) {
        destroyGameWorld( gameWindow->gw );
        free( gameWindow );
    }
}
