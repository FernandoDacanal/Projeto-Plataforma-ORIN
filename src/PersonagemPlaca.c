/**
 * @file Inimigo.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Personagem (Placa).
 *
 * @copyright Copyright (c) 2026
 */
#include <stdlib.h>

#include "include/raylib/raylib.h"

#include "include/Animacao.h"
#include "include/PersonagemPlaca.h"
#include "include/Tipos.h"
#include "include/ResourceManager.h"

static void desenharQuadroAnimacaoPersonagemPlaca( PersonagemPlaca *personagem, QuadroAnimacao *qa, Color tonalidade );
static Animacao *getAnimacaoAtualPersonagemPlaca( PersonagemPlaca *personagem );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria um novo Personagem (peixe).
 */
PersonagemPlaca *criarPersonagemPlaca( Rectangle ret, Color cor ) {

    PersonagemPlaca *novoPersonagem = (PersonagemPlaca*) malloc( sizeof( PersonagemPlaca ) );

    novoPersonagem->ret = ret;
    novoPersonagem->vel = (Vector2) { 0 };
    novoPersonagem->cor = cor;

    novoPersonagem->velAndando = 0;
    novoPersonagem->velMaxQueda = 0;

    novoPersonagem->estado = ESTADO_PERSONAGEM_PLACA_PARADO;
    novoPersonagem->ativo = true;
    novoPersonagem->olhandoParaDireita = false;

    int quantidadeAnimacoes = 0;

    novoPersonagem->animacaoParado.quantidadeQuadros = 1;
    novoPersonagem->animacaoParado.quadroAtual = 0;
    novoPersonagem->animacaoParado.contadorTempoQuadro = 0.0f;
    novoPersonagem->animacaoParado.pararNoUltimoQuadro = false;
    novoPersonagem->animacaoParado.executarUmaVez = false;
    novoPersonagem->animacaoParado.finalizada = false;
    criarQuadrosAnimacao( &novoPersonagem->animacaoParado, novoPersonagem->animacaoParado.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoPersonagem->animacaoParado.quadros,
        novoPersonagem->animacaoParado.quantidadeQuadros,
        120,             // duração padrão para todos os quadros
        0, 0,            // início
        16, 32,          // dimensões
        1,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            0, 16, 16, 16
        }
    );
    novoPersonagem->animacaoInteragivel.quantidadeQuadros = 4;
    novoPersonagem->animacaoInteragivel.quadroAtual = 0;
    novoPersonagem->animacaoInteragivel.contadorTempoQuadro = 0.0f;
    novoPersonagem->animacaoInteragivel.pararNoUltimoQuadro = false;
    novoPersonagem->animacaoInteragivel.executarUmaVez = false;
    novoPersonagem->animacaoInteragivel.finalizada = false;
    criarQuadrosAnimacao( &novoPersonagem->animacaoInteragivel, novoPersonagem->animacaoInteragivel.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoPersonagem->animacaoInteragivel.quadros,
        novoPersonagem->animacaoInteragivel.quantidadeQuadros,
        200,              // duração padrão para todos os quadros
        0, 32,           // início
        16, 32,           // dimensões
        1,                // separação
        false,            // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            0, 16, 16, 16
        }
    );
    novoPersonagem->animacaoFalando.quantidadeQuadros = 2;
    novoPersonagem->animacaoFalando.quadroAtual = 0;
    novoPersonagem->animacaoFalando.contadorTempoQuadro = 0.0f;
    novoPersonagem->animacaoFalando.pararNoUltimoQuadro = false;
    novoPersonagem->animacaoFalando.executarUmaVez = false;
    novoPersonagem->animacaoFalando.finalizada = false;
    criarQuadrosAnimacao( &novoPersonagem->animacaoFalando, novoPersonagem->animacaoFalando.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoPersonagem->animacaoFalando.quadros,
        novoPersonagem->animacaoFalando.quantidadeQuadros,
        250,              // duração padrão para todos os quadros
        0, 0,           // início
        16, 32,           // dimensões
        1,                // separação
        false,            // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            0, 16, 16, 16
        }
    );

    novoPersonagem->animacoes[ESTADO_PERSONAGEM_PLACA_PARADO] = &novoPersonagem->animacaoParado; quantidadeAnimacoes++;
    novoPersonagem->animacoes[ESTADO_PERSONAGEM_PLACA_INTERAGIVEL] = &novoPersonagem->animacaoInteragivel; quantidadeAnimacoes++;
    novoPersonagem->animacoes[ESTADO_PERSONAGEM_PLACA_FALANDO] = &novoPersonagem->animacaoFalando; quantidadeAnimacoes++;
    novoPersonagem->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoPersonagem;
}

void destruirPersonagemPlaca( PersonagemPlaca *personagem ) {
    if ( personagem != NULL ) {
        for ( int i = 0; i < personagem->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( personagem->animacoes[i] );
        }
        free( personagem );
    }
}

/**
 * @brief Atualiza um personagem (peixe).
 */
void atualizarPersonagemPlaca( PersonagemPlaca *personagem, GameWorld *gw, float delta ) {
    if ( personagem->ativo ) {
        Animacao *animacaoAtual = getAnimacaoAtualPersonagemPlaca( personagem );
        atualizarAnimacao( animacaoAtual, delta );
    }
}

/**
 * @brief Desenha um personagem (placa).
 */
void desenharPersonagemPlaca( PersonagemPlaca *personagem ) {
    if ( personagem->ativo ) {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualPersonagemPlaca( personagem );
        desenharQuadroAnimacaoPersonagemPlaca( personagem, qa, WHITE );
    }
}

/**
 * @brief Obtém o quadro de animação atual de um personagem (peixe).
 */

static void desenharQuadroAnimacaoPersonagemPlaca( PersonagemPlaca *personagem, QuadroAnimacao *qa, Color tonalidade ) {
    if ( qa != NULL ) {
        DrawTexturePro(
            rm.texturaNPC,
            (Rectangle) {
                qa->fonte.x,
                qa->fonte.y,
                personagem->olhandoParaDireita ? -qa->fonte.width : qa->fonte.width,
                qa->fonte.height
            },
            personagem->ret,
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULOS ) {
            float xDesenho = personagem->olhandoParaDireita
                ? personagem->ret.x + personagem->ret.width - qa->retColisao.x - qa->retColisao.width
                : personagem->ret.x + qa->retColisao.x;
            float yDesenho = personagem->ret.y + qa->retColisao.y;
            DrawRectangle( xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade( GREEN, 0.5f ) );
        }

    }

}
QuadroAnimacao *getQuadroAnimacaoAtualPersonagemPlaca( PersonagemPlaca *personagem ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualPersonagemPlaca( personagem ) );
}

static Animacao *getAnimacaoAtualPersonagemPlaca( PersonagemPlaca *personagem ) {
    return personagem->animacoes[personagem->estado];
}

