#pragma once
#include "Tipos.h"

#define TEMPO_LIMITE 20
#define TAMANHO_TILES 8

//retangulo sprites hud

#define HUD_SCORE_SRC      (Rectangle){ 0, 0, 40, 16 }
#define HUD_TIME_SRC       (Rectangle){ 0, 16, 56, 16 }
#define HUD_TIME_FLASH_SRC (Rectangle){ 56, 16, 56, 16 }

#define HUD_RING_SRC       (Rectangle){ 0, 32, 56, 16 }
#define HUD_RING_FLASH_SRC (Rectangle){ 56, 32, 56, 16 }

#define HUD_LIVES_SRC      (Rectangle){ 56, 0, 48, 16 }

void desenharSpriteHUD( Rectangle source, Vector2 pos );
void desenharNumero( int valor, Vector2 pos );

void desenharHUD(GameWorld *gw);

//função que definine se os elementos TIME e RINGS da HUD devem estar piscando
void piscarHUD(GameWorld *gw);

void desenharScore(GameWorld *gw);
void desenharTime(GameWorld *gw);
void desenharRings(GameWorld *gw);
void desenharLives(GameWorld *gw);