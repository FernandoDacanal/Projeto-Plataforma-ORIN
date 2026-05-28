#pragma once
#include "Tipos.h"

void desenharHUD(GameWorld *gw);

//função que definine se os elementos TIME e RINGS da HUD devem estar piscando
void piscarHUD(GameWorld *gw);

//função que desenha se os elementos de texto SCORE, TIME, RINGS e LIVES da HUD
void desenharTextoHUD(GameWorld *gw);

void desenharScore(GameWorld *gw);