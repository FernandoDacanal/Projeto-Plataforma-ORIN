/**
 * @file Obstaculo.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções do Inimigo (Motobug).
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo inimigo (motobug).
 */
InimigoMotobug *criarInimigoMotobug( Rectangle ret, Color cor );

/**
 * @brief Destroi um inimigo (motobug).
 */
void destruirInimigoMotobug( InimigoMotobug *inimigo );

/**
 * @brief Atualiza um inimigo (motobug).
 */
void atualizarInimigoMotobug( InimigoMotobug *inimigo, GameWorld *gw,float delta );

/**
 * @brief Desenha um inimigo (motobug).
 */
void desenharInimigoMotobug( InimigoMotobug *inimigo );

/**
 * @brief Obtém o quadro de animação atual de um inimigo (motobug).
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoMotobug( InimigoMotobug *inimigo );