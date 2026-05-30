#pragma once
#include "Tipos.h"

#define TEMPO_LIMITE 20
#define TAMANHO_FONTE 8

//retangulo sprites hud

#define HUD_SCORE_SRC      (Rectangle){ 0, 0, 40, 16 }
#define HUD_TIME_SRC       (Rectangle){ 0, 16, 56, 16 }
#define HUD_TIME_FLASH_SRC (Rectangle){ 56, 16, 56, 16 }

#define HUD_RING_SRC       (Rectangle){ 0, 32, 56, 16 }
#define HUD_RING_FLASH_SRC (Rectangle){ 56, 32, 56, 16 }

#define HUD_LIVES_SRC      (Rectangle){ 56, 0, 48, 16 }
/**
 * Desenha texto como textura na tela.
 *
 * @param string Texto que vai ser desenhado na tela.
 * @param source Posicao e tamanho da fonte.
 * @param rec Posição e tamanho do texto na tela.
 */
void desenharTexto(Texture2D textura, char *string, Rectangle source, Rectangle rec); 

void desenharSpriteHUD( Rectangle source, Vector2 pos );
void desenharNumero( int valor, Vector2 pos );

void desenharHUD(GameWorld *gw);

//função que definine se os elementos TIME e RINGS da HUD devem estar piscando
void piscarHUD(GameWorld *gw);

void desenharScore(GameWorld *gw);
void desenharTime(GameWorld *gw);
void desenharRings(GameWorld *gw);
void desenharLives(GameWorld *gw);