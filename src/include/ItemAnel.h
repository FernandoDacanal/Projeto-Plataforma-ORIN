/**
 * @file Obstaculo.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções do Item (Anel).
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo item (anel).
 */
ItemAnel *criarItemAnel( Rectangle ret, Color cor );

/**
 * @brief Destroi um item (anel).
 */
void destruirItemAnel( ItemAnel *item );

/**
 * @brief Atualiza um item (anel).
 */
void atualizarItemAnel( ItemAnel *item, float delta );

/**
 * @brief Desenha um item (anel).
 */
void desenharItemAnel( ItemAnel *item );

/**
 * @brief Obtém o quadro de animação atual de um item (anel).
 */
QuadroAnimacao *getQuadroAnimacaoAtualItemAnel( ItemAnel *item );