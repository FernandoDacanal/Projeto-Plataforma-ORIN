/**
 * @file Inimigo.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Inimigo (Voador).
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "include/raylib/raylib.h"

#include "include/Animacao.h"
#include "include/Inimigo.h"
#include "include/InimigoVoador.h"
#include "include/Macros.h"
#include "include/ResourceManager.h"
#include "include/Tipos.h"

static void desenharQuadroAnimacaoInimigoVoador( InimigoVoador *inimigo, QuadroAnimacao *qa, Color tonalidade );
static void desenharQuadroAnimacaoInimigoVoadorMorrendo( InimigoVoador *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade );
static Animacao *getAnimacaoAtualInimigoVoador( InimigoVoador *inimigo );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria um novo Inimigo (voador).
 */
InimigoVoador *criarInimigoVoador( Rectangle ret, Color cor ) {

    InimigoVoador *novoInimigo = (InimigoVoador*) malloc( sizeof( InimigoVoador ) );

    novoInimigo->ret = ret;
    novoInimigo->vel = (Vector2) { 0 };
    novoInimigo->cor = cor;

    novoInimigo->velAndando = 100;
    novoInimigo->velMaxQueda = 600;

    novoInimigo->estado = ESTADO_INIMIGO_VOADOR_ANDANDO;
    novoInimigo->ativo = true;
    novoInimigo->olhandoParaDireita = false;

    int quantidadeAnimacoes = 0;

    novoInimigo->animacaoAndando.quantidadeQuadros = 4;
    novoInimigo->animacaoAndando.quadroAtual = 0;
    novoInimigo->animacaoAndando.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoAndando.pararNoUltimoQuadro = false;
    novoInimigo->animacaoAndando.executarUmaVez = false;
    novoInimigo->animacaoAndando.finalizada = false;
    criarQuadrosAnimacao( &novoInimigo->animacaoAndando, novoInimigo->animacaoAndando.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoInimigo->animacaoAndando.quadros,
        novoInimigo->animacaoAndando.quantidadeQuadros,
        60,             // duração padrão para todos os quadros
        1, 100,            // início
        32, 32,          // dimensões
        1,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            6, 16, 20, 12
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
        1, 67,           // início
        32, 32,           // dimensões
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

    novoInimigo->animacoes[ESTADO_INIMIGO_VOADOR_ANDANDO] = &novoInimigo->animacaoAndando; quantidadeAnimacoes++;
    novoInimigo->animacoes[ESTADO_INIMIGO_VOADOR_MORRENDO] = &novoInimigo->animacaoMorrendo; quantidadeAnimacoes++;
    novoInimigo->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoInimigo;

}

/**
 * @brief Destroi um inimigo (voador).
 */
void destruirInimigoVoador( InimigoVoador *inimigo ) {
    if ( inimigo != NULL ) {
        for ( int i = 0; i < inimigo->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( inimigo->animacoes[i] );
        }
        free( inimigo );
    }
}

/**
 * @brief Atualiza um inimigo (voador).
 */
void atualizarInimigoVoador( InimigoVoador *inimigo, GameWorld *gw, float delta ) {

    if ( inimigo->ativo ) {

        if ( inimigo->estado == ESTADO_INIMIGO_VOADOR_ANDANDO ) {

            Animacao *animacaoAtual = getAnimacaoAtualInimigoVoador( inimigo );
            atualizarAnimacao( animacaoAtual, delta );

            Inimigo ini = {
                .objeto = inimigo,
                .tipo = TIPO_INIMIGO_VOADOR
            };

            if ( inimigo->olhandoParaDireita ) {
                inimigo->vel.x = inimigo->velAndando;
            } else {
                inimigo->vel.x = -inimigo->velAndando;
            }
            //esse inimigo nã tem gravidade nem colisão com o mapa
            // fase X
            inimigo->ret.x += inimigo->vel.x * delta;
            /*
            resolverColisaoInimigoObstaculosMapaX( &ini, gw->mapa );
            inimigo->vel.y += gw->gravidade * delta;
            if ( inimigo->vel.y > inimigo->velMaxQueda ) {
                inimigo->vel.y = inimigo->velMaxQueda;
            }
            // fase Y
            inimigo->ret.y += inimigo->vel.y * delta;
            resolverColisaoInimigoObstaculosMapaY( &ini, gw->mapa );
            */
        } else if ( inimigo->estado == ESTADO_INIMIGO_VOADOR_MORRENDO ) {

            atualizarAnimacao( &inimigo->animacaoMorrendo, delta );

            if ( inimigo->animacaoMorrendo.finalizada ) {
                inimigo->ativo = false;
            }

        }

    }

}

/**
 * @brief Desenha um inimigo (voador).
 */
void desenharInimigoVoador( InimigoVoador *inimigo ) {

    if ( inimigo->ativo ) {

        if ( inimigo->estado == ESTADO_INIMIGO_VOADOR_ANDANDO ) {
            QuadroAnimacao *qa = getQuadroAnimacaoAtualInimigoVoador( inimigo );
            desenharQuadroAnimacaoInimigoVoador( inimigo, qa, WHITE );
        } else if ( inimigo->estado == ESTADO_INIMIGO_VOADOR_MORRENDO ) {
            desenharQuadroAnimacaoInimigoVoadorMorrendo( inimigo, getQuadroAtualAnimacao( &inimigo->animacaoMorrendo ), 1.0f, WHITE );
        }

        if ( MOSTRAR_RETANGULOS ) {
            DrawRectangleRec( inimigo->ret, Fade( inimigo->cor, 0.5f ) );
            DrawRectangleLines( inimigo->ret.x, inimigo->ret.y, inimigo->ret.width, inimigo->ret.height, BLACK );
        }

    }

}

/**
 * @brief Obtém o quadro de animação atual de um inimigo (voador).
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoVoador( InimigoVoador *inimigo ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualInimigoVoador( inimigo ) );
}

static void desenharQuadroAnimacaoInimigoVoador( InimigoVoador *inimigo, QuadroAnimacao *qa, Color tonalidade ) {

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

static void desenharQuadroAnimacaoInimigoVoadorMorrendo( InimigoVoador *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade ) {

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

static Animacao *getAnimacaoAtualInimigoVoador( InimigoVoador *inimigo ) {
    return inimigo->animacoes[inimigo->estado];
}