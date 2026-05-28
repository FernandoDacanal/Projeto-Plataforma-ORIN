#include <stdio.h>
#include <stdlib.h>
#include "include/Tipos.h"
#include "include/HUD.h"
#include "include/ResourceManager.h"

#define pouco(var, val) (var != 0 && var < val ? 1 : 0)
#define TEMPO_LIMITE 20
#define TAMANHO_TILES 8

static bool piscar_time = false;
static bool piscar_ring = false;

static float piscar_timeTempo = 0;
static float piscar_ringTempo = 0;

static Vector2 HUDSuperior = {TAMANHO_TILES, TAMANHO_TILES};
static Vector2 HUDInferior = {TAMANHO_TILES, TAMANHO_TILES * 20};

void desenharHUD(GameWorld *gw){
    Vector2 screenPos = GetWorldToScreen2D(gw->jogador->, gw->camera);
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
    if(gw->jogador->quantidadePontos > 9999){
        DrawTexturePro(
            rm.texturaHUD,
            (Rectangle){ (gw->jogador->quantidadePontos / 10000) * TAMANHO_TILES, rm.texturaHUD.height - TAMANHO_TILES * 3, TAMANHO_TILES, TAMANHO_TILES * 2 },
            (Rectangle){ HUDSuperior.x + TAMANHO_TILES * 6, HUDSuperior.y, TAMANHO_TILES, TAMANHO_TILES * 2 },
            (Vector2){ 0 },
            0.f,
            WHITE
        );
    }
    //dezena de milhar
    if(gw->jogador->quantidadePontos > 999){
        DrawTexturePro(
            rm.texturaHUD,
            (Rectangle){ (gw->jogador->quantidadePontos % 10000) / 1000 * TAMANHO_TILES, rm.texturaHUD.height - TAMANHO_TILES * 3, TAMANHO_TILES, TAMANHO_TILES * 2},
            (Rectangle){ HUDSuperior.x + TAMANHO_TILES * 7, HUDSuperior.y, TAMANHO_TILES, TAMANHO_TILES * 2 },
            (Vector2){ 0 },
            0.f,
            WHITE
        );
    }
    //milhar
    if(gw->jogador->quantidadePontos > 99){
        DrawTexturePro(
            rm.texturaHUD,
            (Rectangle){ (gw->jogador->quantidadePontos % 1000) / 100 * TAMANHO_TILES, rm.texturaHUD.height - 24, TAMANHO_TILES, TAMANHO_TILES * 2 },
            (Rectangle){ HUDSuperior.x + TAMANHO_TILES * 8, HUDSuperior.y, TAMANHO_TILES, TAMANHO_TILES * 2 },
            (Vector2){ 0 },
            0.f,
            WHITE
        );
    }
    //centena
    if(gw->jogador->quantidadePontos > 9){
        DrawTexturePro(
            rm.texturaHUD,
            (Rectangle){ (gw->jogador->quantidadePontos % 100) / 10 * 8, rm.texturaHUD.height - TAMANHO_TILES * 3, TAMANHO_TILES, TAMANHO_TILES * 2 },
            (Rectangle){ HUDSuperior.x + TAMANHO_TILES * 9, HUDSuperior.y, TAMANHO_TILES, TAMANHO_TILES * 2 },
            (Vector2){ 0 },
            0.f,
            WHITE
        );
    }
    //dezena
    if(gw->jogador->quantidadePontos > 0){
        DrawTexturePro(
            rm.texturaHUD,
            (Rectangle){ (gw->jogador->quantidadePontos % 10) * 8, rm.texturaHUD.height - TAMANHO_TILES * 3, TAMANHO_TILES, TAMANHO_TILES * 2 },
            (Rectangle){ HUDSuperior.x + TAMANHO_TILES * 10, HUDSuperior.y, TAMANHO_TILES, TAMANHO_TILES * 2 },
            (Vector2){ 0 },
            0.f,
            WHITE
        );
    }
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
    //texto
    DrawTexturePro(
        rm.texturaHUD,
        (Rectangle){piscar_ring * TAMANHO_TILES * 7, TAMANHO_TILES * 4, TAMANHO_TILES * 7, TAMANHO_TILES * 2},
        (Rectangle){HUDSuperior.x, HUDSuperior.y + (TAMANHO_TILES * 4), TAMANHO_TILES * 7, TAMANHO_TILES * 2},
        (Vector2) {0},
        0.0f,
        WHITE
    );
    //centena
    if(gw->jogador->quantidadeAneis > 99){
        DrawTexturePro(
            rm.texturaHUD,
            (Rectangle){ (gw->jogador->quantidadeAneis / 100) * TAMANHO_TILES, rm.texturaHUD.height - TAMANHO_TILES * 3, TAMANHO_TILES, TAMANHO_TILES * 2 },
            (Rectangle){ HUDSuperior.x + (TAMANHO_TILES * 6), HUDSuperior.y + (TAMANHO_TILES * 4), TAMANHO_TILES, TAMANHO_TILES * 2 },
            (Vector2){ 0 },
            0.f,
            WHITE
        );
    }
    //dezena
    if(gw->jogador->quantidadeAneis > 9){
        DrawTexturePro(
            rm.texturaHUD,
            (Rectangle){ (gw->jogador->quantidadeAneis % 100) / 10 * TAMANHO_TILES, rm.texturaHUD.height - TAMANHO_TILES * 3, TAMANHO_TILES, TAMANHO_TILES * 2 },
            (Rectangle){ HUDSuperior.x + (TAMANHO_TILES * 7), HUDSuperior.y + (TAMANHO_TILES * 4), TAMANHO_TILES, TAMANHO_TILES * 2 },
            (Vector2){ 0 },
            0.f,
            WHITE
        );
    }
    //unidade
    DrawTexturePro(
        rm.texturaHUD,
		(Rectangle){ (gw->jogador->quantidadeAneis % 10) * TAMANHO_TILES, rm.texturaHUD.height - TAMANHO_TILES * 3, TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Rectangle){ HUDSuperior.x + (TAMANHO_TILES * 8), HUDSuperior.y + (TAMANHO_TILES * 4), TAMANHO_TILES, TAMANHO_TILES * 2 },
		(Vector2){ 0 },
		0.f,
        WHITE
    );
}

void desenharLives(GameWorld *gw){
    HUDInferior.y = GetScreenHeight() - TAMANHO_TILES * 3;
    //texto
    DrawTexturePro(
        rm.texturaHUD,
        (Rectangle){56, 0, TAMANHO_TILES * 6, TAMANHO_TILES * 2},
        (Rectangle){HUDInferior.x, HUDInferior.y, TAMANHO_TILES * 6, TAMANHO_TILES * 2},
        (Vector2) {0},
        0.0f,
        WHITE
    );
    //dezena
    if(gw->jogador->quantidadeVidas > 9){
        DrawTexturePro(
            rm.texturaHUD, 
            (Rectangle){ gw->jogador->quantidadeVidas / 10 * TAMANHO_TILES, TAMANHO_TILES * 8, TAMANHO_TILES, TAMANHO_TILES },
            (Rectangle){ HUDInferior.x + (TAMANHO_TILES * 4), HUDInferior.y + TAMANHO_TILES, TAMANHO_TILES, TAMANHO_TILES }, 
            (Vector2){ 0 }, 
            0.f, 
            WHITE
        );
    }
    //unidade
	DrawTexturePro(
        rm.texturaHUD, 
        (Rectangle){ gw->jogador->quantidadeVidas % 10 * TAMANHO_TILES, TAMANHO_TILES * 8, TAMANHO_TILES, TAMANHO_TILES },
		(Rectangle){ HUDInferior.x + (TAMANHO_TILES * 5), HUDInferior.y + TAMANHO_TILES, TAMANHO_TILES, TAMANHO_TILES }, 
        (Vector2){ 0 },
        0.f,
        WHITE
    );
}