#include <stdio.h>
#include <stdlib.h>
#include "include/Tipos.h"
#include "include/HUD.h"
#include "include/ResourceManager.h"

#define pouco(var, val) (var != 0 && var < val ? 1 : 0)
#define TEMPO_LIMITE 5
#define TAMANHO_TILES 8

static bool piscar_time = false;
static bool piscar_ring = false;

static float piscar_timeTempo = 0;
static float piscar_ringTempo = 0;

static Vector2 HUDSuperior = {TAMANHO_TILES, TAMANHO_TILES};
static Vector2 HUDInferior = {TAMANHO_TILES, TAMANHO_TILES * 20};

void desenharHUD(GameWorld *gw){
    piscarHUD(gw);
    desenharScore(gw);
    desenharTime(gw);
    desenharRings(gw);
    desenharLives(gw);
}

void piscarHUD(GameWorld *gw){
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
}

void desenharScore(GameWorld *gw){
    DrawTexturePro(
        rm.texturaHUD, 
        (Rectangle){0, 0, TAMANHO_TILES * 5, TAMANHO_TILES * 2},
        (Rectangle){HUDSuperior.x, HUDSuperior.y, TAMANHO_TILES * 5, TAMANHO_TILES * 2},
        (Vector2) {0},
        0.0f,
        WHITE
    );
    Vector2 posValorScore = {112, 16};
    //centena de milhar
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadePontos / 10000) * TAMANHO_TILES, rm.texturaHUD.height - TAMANHO_TILES * 3, TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Rectangle){ HUDSuperior.x + TAMANHO_TILES * 6, HUDSuperior.y, TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //dezena de milhar
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadePontos % 10000) / 1000 * TAMANHO_TILES, rm.texturaHUD.height - TAMANHO_TILES * 3, TAMANHO_TILES, TAMANHO_TILES * 2},
		(Rectangle){ HUDSuperior.x + TAMANHO_TILES * 7, HUDSuperior.y, TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //milhar
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadePontos % 1000) / 100 * TAMANHO_TILES, rm.texturaHUD.height - 24, TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Rectangle){ HUDSuperior.x + TAMANHO_TILES * 8, HUDSuperior.y, TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //centena
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadePontos % 100) / 10 * 8, rm.texturaHUD.height - TAMANHO_TILES * 3, TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Rectangle){ HUDSuperior.x + TAMANHO_TILES * 9, HUDSuperior.y, TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //dezena
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadePontos % 10) * 8, rm.texturaHUD.height - TAMANHO_TILES * 3, TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Rectangle){ HUDSuperior.x + TAMANHO_TILES * 10, HUDSuperior.y, TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //unidade (0)
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ 0, rm.texturaHUD.height - TAMANHO_TILES * 3, TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Rectangle){ HUDSuperior.x + TAMANHO_TILES * 11, HUDSuperior.y, TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
}

void desenharTime(GameWorld *gw){
    int segundos = ((int) gw->jogador->quantidadeTempo % 60);
    int minutos = ((int) gw->jogador->quantidadeTempo / 60);

    Vector2 posTextoTime = {96, 48};

    //texto
    DrawTexturePro(
        rm.texturaHUD,
        (Rectangle){piscar_time * TAMANHO_TILES * 7, TAMANHO_TILES * 2, TAMANHO_TILES * 7, TAMANHO_TILES * 2},
        (Rectangle){HUDSuperior.x, HUDSuperior.y + (TAMANHO_TILES * 2), TAMANHO_TILES * 7, TAMANHO_TILES * 2},
        (Vector2) {0},
        0.0f,
        WHITE
    );
    //minutos
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ minutos * TAMANHO_TILES, rm.texturaHUD.height - TAMANHO_TILES * 3, TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Rectangle){HUDSuperior.x + TAMANHO_TILES * 5, HUDSuperior.y + (TAMANHO_TILES * 2), TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //dezenas de segundos
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ segundos / 10 * 8, rm.texturaHUD.height - TAMANHO_TILES * 3, TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Rectangle){HUDSuperior.x + TAMANHO_TILES * 7, HUDSuperior.y + (TAMANHO_TILES * 2), TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //unidades de segundos
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ segundos % 10 * 8, rm.texturaHUD.height - TAMANHO_TILES * 3, TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Rectangle){HUDSuperior.x + TAMANHO_TILES * 8, HUDSuperior.y + (TAMANHO_TILES * 2), TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
}

void desenharRings(GameWorld *gw){
    Vector2 posTextoRings = {112, 80};
    DrawTexturePro(
        rm.texturaHUD,
        (Rectangle){piscar_ring * TAMANHO_TILES * 7, TAMANHO_TILES * 4, TAMANHO_TILES * 7, TAMANHO_TILES * 2},
        (Rectangle){HUDSuperior.x, HUDSuperior.y + (TAMANHO_TILES * 4), TAMANHO_TILES * 7, TAMANHO_TILES * 2},
        (Vector2) {0},
        0.0f,
        WHITE
    );
    //centena
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadeAneis / 100) * 8, rm.texturaHUD.height - 24, 8, 16 },
		(Rectangle){ posTextoRings.x, posTextoRings.y, 8 * 2, 16 * 2 },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //dezena
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadeAneis % 100) / 10 * 8, rm.texturaHUD.height - 24, 8, 16 },
		(Rectangle){ posTextoRings.x + 16, posTextoRings.y, 8 * 2, 16 * 2 },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
    //unidade
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadeAneis % 10) * 8, rm.texturaHUD.height - 24, 8, 16 },
		(Rectangle){ posTextoRings.x + 16 * 2, posTextoRings.y, 8 * 2, 16 * 2 },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
}

void desenharLives(GameWorld *gw){
    //texto
    DrawTexturePro(
        rm.texturaHUD, 
        (Rectangle){56, 0, 48, 16},
        (Rectangle){16, GetScreenHeight() - 48, 48 * 2, 16 * 2},
        (Vector2) {0},
        0.0f,
        WHITE
    );
    //dezena
    DrawTexturePro(
        rm.texturaHUD, 
        (Rectangle){ gw->jogador->quantidadeVidas / 10 * 8, rm.texturaHUD.height - 8, 8, 8 },
		(Rectangle){ 16 * 6 - 8 * 2, GetScreenHeight() - 32, 8 * 2, 8 * 2 }, 
        (Vector2){ 0 }, 
        0.f, 
        WHITE);
    //unidade
	DrawTexturePro(
        rm.texturaHUD, 
        (Rectangle){ gw->jogador->quantidadeVidas % 10 * 8, rm.texturaHUD.height - 8, 8, 8 },
		(Rectangle){ 16 * 6, GetScreenHeight() - 32, 8 * 2, 8 * 2 }, 
        (Vector2){ 0 },
        0.f,
        WHITE);
}