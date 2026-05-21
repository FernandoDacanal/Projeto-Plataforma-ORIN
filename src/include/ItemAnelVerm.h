/**
 * @file Obstaculo.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções do Item (AnelVerm).
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo item (anelverm).
 */
ItemAnelVerm *criarItemAnelVerm( Rectangle ret, Color cor );

/**
 * @brief Destroi um item (anelverm).
 */
void destruirItemAnelVerm( ItemAnelVerm *item );

/**
 * @brief Atualiza um item (anelverm).
 */
void atualizarItemAnelVerm( ItemAnelVerm *item, float delta );

/**
 * @brief Desenha um item (anelverm).
 */
void desenharItemAnelVerm( ItemAnelVerm *item );

/**
 * @brief Obtém o quadro de animação atual de um item (anelverm).
 */
QuadroAnimacao *getQuadroAnimacaoAtualItemAnelVerm( ItemAnelVerm *item );