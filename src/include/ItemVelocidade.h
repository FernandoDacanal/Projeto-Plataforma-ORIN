/**
 * @file Obstaculo.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções do Item (Velocidade).
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo item (velocidade).
 */
ItemVelocidade *criarItemVelocidade( Rectangle ret, Color cor );

/**
 * @brief Destroi um item (velocidade).
 */
void destruirItemVelocidade( ItemVelocidade *item );

/**
 * @brief Atualiza um item (velocidade).
 */
void atualizarItemVelocidade( ItemVelocidade *item, float delta );

/**
 * @brief Desenha um item (velocidade).
 */
void desenharItemVelocidade( ItemVelocidade *item );

/**
 * @brief Obtém o quadro de animação atual de um item (velocidade).
 */
QuadroAnimacao *getQuadroAnimacaoAtualItemVelocidade( ItemVelocidade *item );