/**
 * @file Obstaculo.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções do Personagem (Placa).
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo personagem (placa).
 */
PersonagemPlaca *criarPersonagemPlaca( Rectangle ret, Color cor );

/**
 * @brief Destroi um personagem (placa).
 */
void destruirPersonagemPlaca( PersonagemPlaca *personagem );

/**
 * @brief Atualiza um personagem (placa).
 */
void atualizarPersonagemPlaca( PersonagemPlaca *personagem, GameWorld *gw,float delta );

/**
 * @brief Desenha um personagem (placa).
 */
void desenharPersonagemPlaca( PersonagemPlaca *personagem );

/**
 * @brief Obtém o quadro de animação atual de um personagem (placa).
 */
QuadroAnimacao *getQuadroAnimacaoAtualPersonagemPlaca( PersonagemPlaca *personagem );