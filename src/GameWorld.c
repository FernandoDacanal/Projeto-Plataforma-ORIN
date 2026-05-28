/**
 * @file GameWorld.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do GameWorld.
 *
 * @copyright Copyright (c) 2026
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/GameWorld.h"
#include "include/Jogador.h"
#include "include/Macros.h"
#include "include/Mapa.h"
#include "include/Obstaculo.h"
#include "include/Tipos.h"
#include "include/ResourceManager.h"
#include "include/Animacao.h"

#include "include/raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h



#define pouco(var, val) (var != 0 && var < val ? 1 : 0)
#define TEMPO_LIMITE 5

static void desenharFundo( GameWorld *gw );
static void atualizarCamera( GameWorld *gw );

static void inicializar( GameWorld *gw );
static void reiniciar( GameWorld *gw );

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

    if ( !IsMusicStreamPlaying( rm.musicaFase01 ) ) {
        PlayMusicStream( rm.musicaFase01 );
    } else {
        UpdateMusicStream( rm.musicaFase01 );
    }

    if ( IsKeyPressed( KEY_R ) ) {
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

    BeginDrawing();
    ClearBackground( (Color) { 36, 0, 180, 255 } );

    BeginMode2D( gw->camera );
    desenharFundo( gw );
    desenharMapa( gw->mapa );
    desenharJogador( gw->jogador );
    EndMode2D();
    
	static bool piscar_time = false;
	static bool piscar_ring = false;

	static float piscar_timeTempo = 0;
	static float piscar_ringTempo = 0;
	if (gw->jogador->quantidadeTempo >= TEMPO_LIMITE && gw->jogador->quantidadeTempo - piscar_timeTempo > 0.5)
	{
		piscar_timeTempo = gw->jogador->quantidadeTempo;
		piscar_time = !piscar_time;
	}
	if (gw->jogador->quantidadeAneis == 0 && gw->jogador->quantidadeTempo - piscar_ringTempo > 0.5 && gw->jogador->quantidadeTempo > 1)
	{
		piscar_ringTempo = gw->jogador->quantidadeTempo;
		piscar_ring = !piscar_ring;
	}
	else if (gw->jogador->quantidadeTempo - piscar_ringTempo > 0.5 && gw->jogador->quantidadeTempo > 1)
		piscar_ring = false;

    DrawTexturePro(
        rm.texturaHUD, 
        (Rectangle){0, 0, 8 * 7, 8 * 2},
        (Rectangle){16, 16, 8 * 7 * 2, 8 * 2 * 2},
        (Vector2) {0},
        0.0f,
        WHITE
    );

    DrawTexturePro(
        rm.texturaHUD,
        (Rectangle){piscar_time * 8 * 7, 16, 8 * 7, 8 * 2},
        (Rectangle){16, 48, 8 * 7 * 2, 8 * 2 * 2},
        (Vector2) {0},
        0.0f,
        WHITE
    );
    DrawTexturePro(
        rm.texturaHUD,
        (Rectangle){piscar_ring * 8 * 7, 32, 8 * 7, 8 * 2},
        (Rectangle){16 + pouco(gw->jogador->contadorTempoInvulnerabilidade, .3f) * rand() % 10,
		80 + pouco(gw->jogador->contadorTempoInvulnerabilidade, .3f) * rand() % 5, 8 * 7 * 2, 8 * 2 * 2},
        (Vector2) {0},
        0.0f,
        WHITE
    );

    int segundos = ((int) gw->jogador->quantidadeTempo % 60);
    int minutos = ((int) gw->jogador->quantidadeTempo / 60);

    int escala = 2;
    int esp = 8 * escala;
    Vector2 posTextoScore = {112, 16};
    Vector2 posTextoTime = {96, 48};
    Vector2 posTextoRings = {112, 80};
    Vector2 posTextoLives = {112, 128};

    
    //Inicio desenho HUD pontos
    //centena de milhar
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadePontos / 10000) * 8, rm.texturaHUD.height - 24, 8, 16 },
		(Rectangle){ posTextoScore.x, posTextoScore.y, 8 * escala, 16 * escala },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //dezena de milhar
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadePontos % 10000) / 1000 * 8, rm.texturaHUD.height - 24, 8, 16 },
		(Rectangle){ posTextoScore.x + esp, posTextoScore.y, 8 * escala, 16 * escala },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //milhar
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadePontos % 1000) / 100 * 8, rm.texturaHUD.height - 24, 8, 16 },
		(Rectangle){ posTextoScore.x + (esp * 2), posTextoScore.y, 8 * escala, 16 * escala },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //centena
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadePontos % 100) / 10 * 8, rm.texturaHUD.height - 24, 8, 16 },
		(Rectangle){ posTextoScore.x + (esp * 3), posTextoScore.y, 8 * escala, 16 * escala },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //dezena
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadePontos % 10) * 8, rm.texturaHUD.height - 24, 8, 16 },
		(Rectangle){ posTextoScore.x + (esp * 4), posTextoScore.y, 8 * 2, 16 * 2 },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //unidade (0)
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ 0, rm.texturaHUD.height - 24, 8, 16 },
		(Rectangle){ posTextoScore.x + (esp * 5), posTextoScore.y, 8 * 2, 16 * 2 },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //Fim desenho pontos

    //Inicio do desenho do HUD tempo
    //minutos
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ minutos * 8, rm.texturaHUD.height - 24, 8, 16 },
		(Rectangle){ posTextoTime.x, posTextoTime.y, 8 * escala, 16 * escala },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //dezenas de segundos
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ segundos / 10 * 8, rm.texturaHUD.height - 24, 8, 16 },
		(Rectangle){ posTextoTime.x + esp * 2, posTextoTime.y, 8 * escala, 16 * escala },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //unidades de segundos
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ segundos % 10 * 8, rm.texturaHUD.height - 24, 8, 16 },
		(Rectangle){ posTextoTime.x + esp * 3, posTextoTime.y, 8 * escala, 16 * escala },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //Fim desenho tempo
    
    //Inicio desenho HUD aneis
    //centena
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadeAneis / 100) * 8, rm.texturaHUD.height - 24, 8, 16 },
		(Rectangle){ posTextoRings.x, posTextoRings.y, 8 * escala, 16 * escala },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //dezena
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadeAneis % 100) / 10 * 8, rm.texturaHUD.height - 24, 8, 16 },
		(Rectangle){ posTextoRings.x + esp, posTextoRings.y, 8 * escala, 16 * escala },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //unidade
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadeAneis % 10) * 8, rm.texturaHUD.height - 24, 8, 16 },
		(Rectangle){ posTextoRings.x + esp * 2, posTextoRings.y, 8 * escala, 16 * escala },
		(Vector2){ 0 },
		0.f,
        WHITE
    );

    //Inicio desenhar vidas
    DrawTexturePro(
        rm.texturaHUD, 
        (Rectangle){56, 0, 48, 16},
        (Rectangle){16, GetScreenHeight() - 48, 48 * escala, 16 * escala},
        (Vector2) {0},
        0.0f,
        WHITE
    );
	int valor;
	if (gw->jogador->quantidadeVidas % 10 * 8 < 0)
		valor = 0;
	else
		valor = gw->jogador->quantidadeVidas % 10 * 8;
	DrawTexturePro(rm.texturaHUD, (Rectangle){ valor, rm.texturaHUD.height - 8, 8, 8 },
			(Rectangle){ 16 * 6, GetScreenHeight() - 32, 8 * 2, 8 * 2 }, (Vector2){ 0 }, 0.f, WHITE);

	DrawTexturePro(rm.texturaHUD, (Rectangle){ gw->jogador->quantidadeVidas / 10 * 8, rm.texturaHUD.height - 8, 8, 8 },
			(Rectangle){ 16 * 6 - 8 * 2, GetScreenHeight() - 32, 8 * 2, 8 * 2 }, (Vector2){ 0 }, 0.f, WHITE);
    //Final desenhar vidas
    
    //DEBUG
    /*
    DrawText( TextFormat( "Anéis: %d", gw->jogador->quantidadeAneis ), 10, 10, 20, ORANGE );
    DrawText( TextFormat( "Vidas: %d", gw->jogador->quantidadeVidas ), 10, 30, 20, ORANGE );
    DrawText( 
        TextFormat( 
            "Invulnerável: %s%s", 
            gw->jogador->invulneravel ? "sim" : "não",
            gw->jogador->invulneravel ? TextFormat( " (%.2fs/%.2fs)", gw->jogador->contadorTempoInvulnerabilidade, gw->jogador->tempoInvulnerabilidade ) : ""
        ), 
        10, 50, 20, ORANGE
    );
    DrawFPS( 10, 70 );
    DrawText(TextFormat("Tempo: %.0f", gw->jogador->quantidadeTempo), 10, 90, 20, ORANGE);
    */
    
    EndDrawing();

}

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

    c->offset.x = GetScreenWidth() / 2;
    c->offset.y = GetScreenHeight() / 2;

    // O target é arredondado para o inteiro mais próximo para garantir que a
    // translação da câmera ocorra sempre em posições inteiras de pixel. Sem esse
    // arredondamento, o valor float contínuo de ret.x faz os tiles serem
    // renderizados em posições subpixel, causando frestas visíveis entre eles.
    c->target.x = roundf( j->ret.x + j->ret.width / 2.0f );
    c->target.y = roundf( j->ret.y + j->ret.height / 2.0f );

    int minX = GetScreenWidth() / 2;
    int maxX = calcularLarguraMapa( gw->mapa ) - GetScreenWidth() / 2;
    int maxY = calcularAlturaMapa( gw->mapa ) - GetScreenHeight() / 2;

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

    //gw->mapa = carregarMapa( "resources/mapas/mapaTeste.txt" );
    gw->mapa = carregarMapa( "resources/mapas/mapa01.txt" );
    gw->jogador = criarJogador( GetScreenWidth() / 2 + 144, calcularAlturaMapa( gw->mapa ) - 196, 96, 96 );

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
