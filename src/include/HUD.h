#pragma once
#include "Tipos.h"

#define TEMPO_LIMITE 20
#define TAMANHO_FONTE 8
#define BORDA 3

#define HUD_FONTE (Rectangle){0, 0, 8, 8}

//tá bom de efeito
enum {
	SEM_EFEITO, 
    TREMER, 
    ONDA,
    ITALICO,
    AUMENTARX,
    AUMENTARY
};

/**
 * Desenha texto como textura na tela.
 *
 * @param string Texto que vai ser desenhado na tela.
 * @param source Posicao e tamanho da fonte.
 * @param rec Posição e tamanho do texto na tela.
 */
void desenharHUD(GameWorld *gw);

void desenharScore(GameWorld *gw);
void desenharTime(GameWorld *gw);
void desenharRings(GameWorld *gw);
void desenharLives(GameWorld *gw);
void desenharBorda(Color cor1, Color cor2);
