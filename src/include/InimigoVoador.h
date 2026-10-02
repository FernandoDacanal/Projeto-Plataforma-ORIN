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
InimigoVoador *criarInimigoVoador( Rectangle ret, Color cor );

/**
 * @brief Destroi um inimigo (motobug).
 */
void destruirInimigoVoador( InimigoVoador *inimigo );

/**
 * @brief Atualiza um inimigo (motobug).
 */
void atualizarInimigoVoador( InimigoVoador *inimigo, GameWorld *gw,float delta );

/**
 * @brief Desenha um inimigo (motobug).
 */
void desenharInimigoVoador( InimigoVoador *inimigo );

/**
 * @brief Obtém o quadro de animação atual de um inimigo (motobug).
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoVoador( InimigoVoador *inimigo );