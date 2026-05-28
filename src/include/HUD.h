#pragma once
#include "Tipos.h"

void desenharHUD(GameWorld *gw);

//função que definine se os elementos TIME e RINGS da HUD devem estar piscando
void piscarHUD(GameWorld *gw);

void desenharScore(GameWorld *gw);
void desenharTime(GameWorld *gw);
void desenharRings(GameWorld *gw);
void desenharLives(GameWorld *gw);