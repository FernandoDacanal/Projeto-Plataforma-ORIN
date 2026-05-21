/**
 * @file Obstaculo.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções do Inimigo (Spikes).
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo inimigo (Spikes).
 */
InimigoSpikes *criarInimigoSpikes( Rectangle ret, Color cor );

/**
 * @brief Destroi um inimigo (Spikes).
 */
void destruirInimigoSpikes( InimigoSpikes *inimigo );

/**
 * @brief Atualiza um inimigo (Spikes).
 */
void atualizarInimigoSpikes( InimigoSpikes *inimigo, GameWorld *gw,float delta );

/**
 * @brief Desenha um inimigo (Spikes).
 */
void desenharInimigoSpikes( InimigoSpikes *inimigo );

/**
 * @brief Obtém o quadro de animação atual de um inimigo (Spikes).
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoSpikes( InimigoSpikes *inimigo );