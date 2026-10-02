/**
 * @file Inimigo.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Inimigo (Peixe).
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "include/raylib/raylib.h"

#include "include/Animacao.h"
#include "include/Inimigo.h"
#include "include/InimigoPeixe.h"
#include "include/Macros.h"
#include "include/ResourceManager.h"
#include "include/Tipos.h"

static void desenharQuadroAnimacaoInimigoPeixe( InimigoPeixe *inimigo, QuadroAnimacao *qa, Color tonalidade );
static void desenharQuadroAnimacaoInimigoPeixeMorrendo( InimigoPeixe *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade );
static Animacao *getAnimacaoAtualInimigoPeixe( InimigoPeixe *inimigo );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria um novo Inimigo (peixe).
 */
InimigoPeixe *criarInimigoPeixe( Rectangle ret, Color cor ) {

    InimigoPeixe *novoInimigo = (InimigoPeixe*) malloc( sizeof( InimigoPeixe ) );

    novoInimigo->ret = ret;
    novoInimigo->vel = (Vector2) { 0 };
    novoInimigo->cor = cor;

    novoInimigo->velAndando = 800;
    novoInimigo->velMaxQueda = 600;

    novoInimigo->estado = ESTADO_INIMIGO_PEIXE_ANDANDO;
    novoInimigo->ativo = true;
    novoInimigo->olhandoParaDireita = false;

    int quantidadeAnimacoes = 0;

    novoInimigo->animacaoAndando.quantidadeQuadros = 2;
    novoInimigo->animacaoAndando.quadroAtual = 0;
    novoInimigo->animacaoAndando.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoAndando.pararNoUltimoQuadro = false;
    novoInimigo->animacaoAndando.executarUmaVez = false;
    novoInimigo->animacaoAndando.finalizada = false;
    criarQuadrosAnimacao( &novoInimigo->animacaoAndando, novoInimigo->animacaoAndando.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoInimigo->animacaoAndando.quadros,
        novoInimigo->animacaoAndando.quantidadeQuadros,
        120,             // duração padrão para todos os quadros
        18, 182,            // início
        16, 16,          // dimensões
        1,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            0, 0, 16, 16
        }
    );

    novoInimigo->animacaoMorrendo.quantidadeQuadros = 3;
    novoInimigo->animacaoMorrendo.quadroAtual = 0;
    novoInimigo->animacaoMorrendo.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoMorrendo.pararNoUltimoQuadro = false;
    novoInimigo->animacaoMorrendo.executarUmaVez = true;
    novoInimigo->animacaoMorrendo.finalizada = false;
    criarQuadrosAnimacao( &novoInimigo->animacaoMorrendo, novoInimigo->animacaoMorrendo.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoInimigo->animacaoMorrendo.quadros,
        novoInimigo->animacaoMorrendo.quantidadeQuadros,
        100,              // duração padrão para todos os quadros
        52, 182,           // início
        16, 16,           // dimensões
        1,                // separação
        false,            // de trás para frente
        (Rectangle) { 0 } // retângulo de colisão padrão para cada quadro
    );

    novoInimigo->animacaoMirando.quantidadeQuadros = 2;
    novoInimigo->animacaoMirando.quadroAtual = 0;
    novoInimigo->animacaoMirando.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoMirando.pararNoUltimoQuadro = false;
    novoInimigo->animacaoMirando.executarUmaVez = true;
    novoInimigo->animacaoMirando.finalizada = false;
    criarQuadrosAnimacao( &novoInimigo->animacaoMirando, novoInimigo->animacaoMirando.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoInimigo->animacaoMirando.quadros,
        novoInimigo->animacaoMirando.quantidadeQuadros,
        100,              // duração padrão para todos os quadros
        1, 67,           // início
        32, 32,           // dimensões
        1,                // separação
        false,            // de trás para frente
        (Rectangle) { 0 } // retângulo de colisão padrão para cada quadro
    );

    novoInimigo->animacoes[ESTADO_INIMIGO_PEIXE_ANDANDO] = &novoInimigo->animacaoAndando; quantidadeAnimacoes++;
    novoInimigo->animacoes[ESTADO_INIMIGO_PEIXE_MORRENDO] = &novoInimigo->animacaoMorrendo; quantidadeAnimacoes++;
    novoInimigo->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoInimigo;

}

/**
 * @brief Destroi um inimigo (peixe).
 */
void destruirInimigoPeixe( InimigoPeixe *inimigo ) {
    if ( inimigo != NULL ) {
        for ( int i = 0; i < inimigo->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( inimigo->animacoes[i] );
        }
        free( inimigo );
    }
}

/**
 * @brief Atualiza um inimigo (peixe).
 */
void atualizarInimigoPeixe( InimigoPeixe *inimigo, GameWorld *gw, float delta ) {

    if ( inimigo->ativo ) {

        if ( inimigo->estado == ESTADO_INIMIGO_PEIXE_ANDANDO ) {

            Animacao *animacaoAtual = getAnimacaoAtualInimigoPeixe( inimigo );
            atualizarAnimacao( animacaoAtual, delta );

            Inimigo ini = {
                .objeto = inimigo,
                .tipo = TIPO_INIMIGO_PEIXE
            };
        
            if ( inimigo->olhandoParaDireita ) {
                inimigo->vel.x = inimigo->velAndando;
            } else {
                inimigo->vel.x = -inimigo->velAndando;
            }
            
            
        } else if ( inimigo->estado == ESTADO_INIMIGO_PEIXE_MORRENDO ) {

            atualizarAnimacao( &inimigo->animacaoMorrendo, delta );

            if ( inimigo->animacaoMorrendo.finalizada ) {
                inimigo->ativo = false;
            }

        }

    }

}

/**
 * @brief Desenha um inimigo (peixe).
 */
void desenharInimigoPeixe( InimigoPeixe *inimigo ) {

    if ( inimigo->ativo ) {

        if ( inimigo->estado == ESTADO_INIMIGO_PEIXE_ANDANDO ) {
            QuadroAnimacao *qa = getQuadroAnimacaoAtualInimigoPeixe( inimigo );
            desenharQuadroAnimacaoInimigoPeixe( inimigo, qa, WHITE );
        } else if ( inimigo->estado == ESTADO_INIMIGO_PEIXE_MORRENDO ) {
            desenharQuadroAnimacaoInimigoPeixeMorrendo( inimigo, getQuadroAtualAnimacao( &inimigo->animacaoMorrendo ), 1.0f, WHITE );
        }

        if ( MOSTRAR_RETANGULOS ) {
            DrawRectangleRec( inimigo->ret, Fade( inimigo->cor, 0.5f ) );
            DrawRectangleLines( inimigo->ret.x, inimigo->ret.y, inimigo->ret.width, inimigo->ret.height, BLACK );
        }

    }

}

/**
 * @brief Obtém o quadro de animação atual de um inimigo (peixe).
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoPeixe( InimigoPeixe *inimigo ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualInimigoPeixe( inimigo ) );
}

static void desenharQuadroAnimacaoInimigoPeixe( InimigoPeixe *inimigo, QuadroAnimacao *qa, Color tonalidade ) {

    if ( qa != NULL ) {
        
        DrawTexturePro(
            rm.texturaBadniks,
            (Rectangle) {
                qa->fonte.x,
                qa->fonte.y,
                inimigo->olhandoParaDireita ? -qa->fonte.width : qa->fonte.width,
                qa->fonte.height
            },
            inimigo->ret,
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULOS ) {
            float xDesenho = inimigo->olhandoParaDireita
                ? inimigo->ret.x + inimigo->ret.width - qa->retColisao.x - qa->retColisao.width
                : inimigo->ret.x + qa->retColisao.x;
            float yDesenho = inimigo->ret.y + qa->retColisao.y;
            DrawRectangle( xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade( GREEN, 0.5f ) );
        }

    }

}

static void desenharQuadroAnimacaoInimigoPeixeMorrendo( InimigoPeixe *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade ) {

    if ( qa != NULL ) {
        
        DrawTexturePro(
            rm.texturaBadniks,
            qa->fonte,
            (Rectangle) {
                inimigo->ret.x,
                inimigo->ret.y,
                qa->fonte.width * escala,
                qa->fonte.height * escala
            },
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

    }

}

static Animacao *getAnimacaoAtualInimigoPeixe( InimigoPeixe *inimigo ) {
    return inimigo->animacoes[inimigo->estado];
}