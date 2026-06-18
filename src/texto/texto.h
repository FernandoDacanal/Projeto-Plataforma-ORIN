#pragma once
#include "../include/raylib.h"
#include "../include/ResourceManager.h"
#include "../include/Tipos.h"

#define inicializar_fonte fonte_tam = { 0, 0, 8, 8 }; fonte = rm

extern Rectangle fonte_tam;	// Tamanho dos caracteres
extern ResourceManager fonte;

// void desenharTexto(char* str, int x, int y);
void desenharTexto(char* str, int x, int y, ...);

void testeTexto();

int efeitoTremer(int limite);
int efeitoOnda(int limite, int i);
