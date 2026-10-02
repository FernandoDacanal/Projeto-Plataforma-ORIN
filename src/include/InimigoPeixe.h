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
InimigoPeixe *criarInimigoPeixe( Rectangle ret, Color cor );

/**
 * @brief Destroi um inimigo (motobug).
 */
void destruirInimigoPeixe( InimigoPeixe *inimigo );

/**
 * @brief Atualiza um inimigo (motobug).
 */
void atualizarInimigoPeixe( InimigoPeixe *inimigo, GameWorld *gw,float delta );

/**
 * @brief Desenha um inimigo (motobug).
 */
void desenharInimigoPeixe( InimigoPeixe *inimigo );

/**
 * @brief Obtém o quadro de animação atual de um inimigo (motobug).
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoPeixe( InimigoPeixe *inimigo );