/**
 * @file GameWorld.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do GameWorld.
 *
 * @copyright Copyright (c) 2026
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/GameWorld.h"
#include "include/GameWindow.h"
#include "include/Jogador.h"
#include "include/Mapa.h"
#include "include/Tipos.h"
#include "include/ResourceManager.h"
#include "include/HUD.h"
#include "include/Utils.h"

#include "include/raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

#include "extras/dev.h"
#include "texto/texto.h"
#include "include/nivel.h"

unsigned char mapaAtual = MAPA1;

static void desenharFundo( GameWorld *gw );
static void atualizarCamera( GameWorld *gw );

static void inicializar( GameWorld *gw );
static void reiniciar( GameWorld *gw );

RenderTexture2D alvoRenderizacao;

bool musica_ativa = true;	// Eu sei que não é legal ficar fazendo variável global ... :)



/**
 * @brief Cria uma instância alocada dinamicamente da struct GameWorld.
 */
GameWorld *createGameWorld( void ) {
    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );
    inicializar( gw );
    return gw;
}

/**
 * @brief Destrói um objeto GameWorld e suas dependências.
 */
void destroyGameWorld( GameWorld *gw ) {
    if ( gw != NULL ) {
        destruirMapa( gw->mapa );
        destruirJogador( gw->jogador );
        free( gw );
    }
}

/**
 * @brief Lê a entrada do usuário e atualiza o estado do jogo.
 */
void updateGameWorld( GameWorld *gw, float delta ) {
    if (IsKeyPressed(KEY_M))
		musica_ativa = !musica_ativa;

	if (mod_desenvolvedor)
	{
		// 1 e 2 resetam tudo
		if (IsKeyPressed(KEY_ONE))
		{
			gw->cor_fundo = AZULCLARO;
			destruirMapa(gw->mapa);
			gw->mapa = carregarMapa("resources/mapas/mapa01.txt");
			UnloadTexture(rm.texturaTerreno);
			rm.texturaTerreno = LoadTexture("resources/imagens/tiles/terreno1.png");
			UnloadMusicStream(rm.musicaFase01);
			rm.musicaFase01 = LoadMusicStream( "resources/sons/musicas/green-hill-zone.mp3" );
			gw->jogador = criarJogador(310, 208, 32, 32);
			mapaAtual = MAPA1;
		}
		else if (IsKeyPressed(KEY_TWO))
		{
			gw->cor_fundo = AMARELO;
			destruirMapa(gw->mapa);
			gw->mapa = carregarMapa("resources/mapas/mapa02.txt");
			UnloadTexture(rm.texturaTerreno);
			rm.texturaTerreno = carregarTexturaAlterandoCores("resources/imagens/tiles/terreno2.png", FUNDO, (Color[]) {BLANK}, 3);
			UnloadMusicStream(rm.musicaFase01);
			rm.musicaFase01 = LoadMusicStream( "resources/sons/musicas/desert-hill.mp3" );
			gw->jogador = criarJogador(22, 208, 32, 32);
			mapaAtual = MAPA2;
		}
		// Muda para a próxima fase e matém todas as outras coisas
		else if (IsKeyPressed(KEY_THREE))
		{
			MudarFase(gw, MAPA2);
		}
	}

	if (musica_ativa)
	{
		if ( !IsMusicStreamPlaying( rm.musicaFase01 ) ) {
			PlayMusicStream( rm.musicaFase01 );
		} else {
			UpdateMusicStream( rm.musicaFase01 );
		}
	}

    if (IsKeyPressed( KEY_R ))
	{
        reiniciar( gw );
        return;
    }

    Jogador *j = gw->jogador;
    atualizarMapa( gw->mapa, gw, delta );
    entradaJogador( j, delta );
    atualizarJogador( j, gw, delta );
    atualizarCamera( gw );
}

/**
 * @brief Desenha o estado do jogo.
 */
void drawGameWorld( GameWorld *gw ) {
	ClearBackground(gw->cor_fundo);

    //elementos alterados pela camera
	BeginMode2D( gw->camera );
	//desenharFundo( gw );
	desenharMapa( gw->mapa );
	desenharJogador( gw->jogador );
	EndMode2D();

	desenharHUD(gw);
    
	//DEBUG
	if (mod_desenvolvedor)
	{
		desenharTexto("[a]FPS: %[/]", 5, 5, GetFPS());
		desenharTexto("[a]x: %[/]", 5, 13, (int)gw->jogador->ret.x);
		desenharTexto("[a]y: %[/]", 5, 21, (int)gw->jogador->ret.y);
		if (gw->jogador->invulneravel)
			desenharTexto("[a]Invulnerável: %[/]", 5, 29, gw->jogador->invulneravel);
	}

	if (gw->jogador->acelerado)
		desenharTexto("[c]{t}{c}ACELERADO!!!{/c}{/t}[/]", 10, 10);

	//testeTexto();
	
	//desenharTexto("{c}{t}contorno{/t}{/c}", 50, 50);
	//desenharTexto("{x4}xy{/x}{y3}y3{/y}", 56, 50);
}

// TODO: No momento não está sendo utilizado. Remover se não for usar.
// A gente vai usar depois, só tenho que fazer um fundo novo primeiro
static void desenharFundo( GameWorld *gw ) {

    int larguraFundo = rm.texturaFundo.width;
    int larguraMapa = calcularLarguraMapa( gw->mapa );
    int alturaMapa = calcularAlturaMapa( gw->mapa );
    int repeticoes = larguraMapa / larguraFundo;

    int deslocamentoParallax = (int) ( ( gw->camera.target.x / (float) larguraMapa ) * -200 );

    for ( int i = 0; i <= repeticoes; i++ ) {
        DrawTexture( rm.texturaFundo, larguraFundo * i - deslocamentoParallax, alturaMapa - rm.texturaFundo.height, WHITE );
    }
}

static void atualizarCamera( GameWorld *gw ) {

    Jogador *j = gw->jogador;
    Camera2D *c = &gw->camera;

    c->offset.x = (int)(LARGURA_VIRTUAL / 2);
    c->offset.y = (int)(ALTURA_VIRTUAL / 2 - 2);

    // O target é arredondado para o inteiro mais próximo para garantir que a
    // translação da câmera ocorra sempre em posições inteiras de pixel. Sem esse
    // arredondamento, o valor float contínuo de ret.x faz os tiles serem
    // renderizados em posições subpixel, causando frestas visíveis entre eles.
    c->target.x = roundf( j->ret.x + j->ret.width / 2.0f );
    c->target.y = roundf( j->ret.y + j->ret.height / 2.0f );

    int minX = LARGURA_VIRTUAL / 2;
    int maxX = calcularLarguraMapa( gw->mapa ) - LARGURA_VIRTUAL / 2;
    int maxY = calcularAlturaMapa( gw->mapa ) - ALTURA_VIRTUAL / 2 - 16;

    if ( c->target.x < minX ) {
        c->target.x = minX;
    } else if ( c->target.x > maxX ) {
        c->target.x = maxX;
    }

    if ( c->target.y > maxY ) {
        c->target.y = maxY;
    }

}

static void inicializar( GameWorld *gw ) {
	if (mod_desenvolvedor)
		musica_ativa = false;

	gw->cor_fundo = AZULCLARO;

    gw->mapa = carregarMapa( "resources/mapas/mapa01.txt" );
    // gw->jogador = criarJogador( (float)GetScreenWidth() / 2 + 144, calcularAlturaMapa( gw->mapa ) - 196, 32, 32 );
    gw->jogador = criarJogador( 313, 208, 32, 32 );

    gw->camera = (Camera2D) {
        .offset = { 0 },    // deslocamento relativo da câmera em relação ao alvo
        .target = { 0 },    // o alvo da câmera, ou seja, a coordenada em que ela está centralizada
        .rotation = 0.0f,   // rotação da câmera em graus. o pivô é o alvo.
        .zoom = 1.0f        // zoom da câmera. 1.0f significa sem escala
    };
    gw->gravidade = 900;

}

static void reiniciar( GameWorld *gw ) {

    destruirMapa( gw->mapa );
    destruirJogador( gw->jogador );

    if ( IsMusicStreamPlaying( rm.musicaFase01 ) ) {
        StopMusicStream( rm.musicaFase01 );
    }

    inicializar( gw );
}
