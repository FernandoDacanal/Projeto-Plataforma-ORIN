/**
 * @file Utils.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções utilitárias.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "raylib/raylib.h"

/**
 * @brief Carrega uma textura trocando cores.
 */
Texture2D carregarTexturaAlterandoCores( const char *caminhoArquivo, Color *coresFonte, Color *coresAlvo, int quantidadeCores );

int tremer(int limite);

#define PRETO       (Color){  35,  35,  35, 255}
#define ROXO        (Color){  93,  39,  93, 255}
#define VERMELHO    (Color){ 177,  62,  83, 255}
#define LARANJA     (Color){ 239, 125,  87, 255}
#define AMARELO     (Color){ 255, 205, 117, 255}
#define VERDECLARO  (Color){ 167, 240, 112, 255}
#define VERDE       (Color){  56, 183, 100, 255}
#define VERDEESCURO (Color){  37, 113, 121, 255}
#define AZULESCURO  (Color){  41,  54, 111, 255}
#define AZUL        (Color){  59,  93, 201, 255}
#define AZULCLARO   (Color){  65, 166, 246, 255}
#define CIANO       (Color){ 115, 239, 247, 255}
#define BRANCO      (Color){ 244, 244, 244, 244}
#define CINZACLARO  (Color){ 148, 176, 194, 255}
#define CINZA       (Color){  86, 108, 134, 255}
#define CINZAESCURO (Color){  51,  60,  87, 255}

#define FUNDO (Color[]) {{ 255, 0, 0, 255 },{ 0, 255, 0, 255 },{ 0, 0, 255, 255 }}