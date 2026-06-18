#pragma once
#include "../include/raylib.h"
#include "../include/ResourceManager.h"

#define inicializar_fonte fonte_tam = { 0, 0, 8, 8 }; fonte = rm

extern Rectangle fonte_tam;	// Tamanho dos caracteres
extern ResourceManager fonte;

// void desenharTexto(char* str, int x, int y);
void desenharTexto(char* str, int x, int y, ...);
void desenharDialogo(char *texto, int x, int y, int caracteresPorLinha, int linhasPorPagina, int paginaAtual);
int obterTotalPaginas(char *texto, int caracteresPorLinha, int linhasPorPagina);

void testeTexto();

int efeitoTremer(int limite);
int efeitoOnda(int limite, int i);
