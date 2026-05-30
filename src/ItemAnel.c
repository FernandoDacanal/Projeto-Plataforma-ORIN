/**
 * @file Item.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Item (Anel).
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "include/raylib/raylib.h"

#include "include/Animacao.h"
#include "include/ItemAnel.h"
#include "include/Macros.h"
#include "include/ResourceManager.h"
#include "include/Tipos.h"

static void desenharQuadroAnimacaoItemAnel( ItemAnel *item, QuadroAnimacao *qa, Color tonalidade );
static Animacao *getAnimacaoAtualItemAnel( ItemAnel *item );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria um novo Item (anel).
 */
ItemAnel *criarItemAnel( Rectangle ret, Color cor ) {

    ItemAnel *novoItem = (ItemAnel*) malloc( sizeof( ItemAnel ) );

    novoItem->ret = ret;
    novoItem->cor = cor;
    novoItem->estado = ESTADO_ITEM_ANEL_PARADO;
    novoItem->ativo = true;

    int quantidadeAnimacoes = 0;

    novoItem->animacaoParado.quantidadeQuadros = 4;
    novoItem->animacaoParado.quadroAtual = 0;
    novoItem->animacaoParado.contadorTempoQuadro = 0.0f;
    novoItem->animacaoParado.pararNoUltimoQuadro = false;
    novoItem->animacaoParado.executarUmaVez = false;
    novoItem->animacaoParado.finalizada = false;
    criarQuadrosAnimacao( &novoItem->animacaoParado, novoItem->animacaoParado.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoItem->animacaoParado.quadros,
        novoItem->animacaoParado.quantidadeQuadros,
        100,             // duração padrão para todos os quadros
        1, 1,            // início
        16, 16,          // dimensões
        1,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            0, 0, 16, 16
        }
    );

    novoItem->animacaoColetando.quantidadeQuadros = 4;
    novoItem->animacaoColetando.quadroAtual = 0;
    novoItem->animacaoColetando.contadorTempoQuadro = 0.0f;
    novoItem->animacaoColetando.pararNoUltimoQuadro = false;
    novoItem->animacaoColetando.executarUmaVez = true;
    novoItem->animacaoColetando.finalizada = false;
    criarQuadrosAnimacao( &novoItem->animacaoColetando, novoItem->animacaoColetando.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoItem->animacaoColetando.quadros,
        novoItem->animacaoColetando.quantidadeQuadros,
        80,               // duração padrão para todos os quadros
        1, 18,            // início
        16, 16,           // dimensões
        1,                // separação
        false,            // de trás para frente
        (Rectangle) { 0 } // retângulo de colisão padrão para cada quadro
    );

    novoItem->animacoes[ESTADO_ITEM_ANEL_PARADO] = &novoItem->animacaoParado; quantidadeAnimacoes++;
    novoItem->animacoes[ESTADO_ITEM_ANEL_COLETADO] = &novoItem->animacaoColetando; quantidadeAnimacoes++;
    novoItem->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoItem;

}

/**
 * @brief Destroi um item (anel).
 */
void destruirItemAnel( ItemAnel *item ) {
    if ( item != NULL ) {
        for ( int i = 0; i < item->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( item->animacoes[i] );
        }
        free( item );
    }
}

/**
 * @brief Atualiza um item (anel).
 */
void atualizarItemAnel( ItemAnel *item, float delta ) {
    if ( item->ativo ) {
        Animacao *animacaoAtual = getAnimacaoAtualItemAnel( item );
        atualizarAnimacao( animacaoAtual, delta );
        if ( item->estado == ESTADO_ITEM_ANEL_COLETADO && animacaoAtual->finalizada ) {
            item->ativo = false;
        }
    }
}

/**
 * @brief Desenha um item (anel).
 */
void desenharItemAnel( ItemAnel *item ) {
    if ( item->ativo ) {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualItemAnel( item );
        desenharQuadroAnimacaoItemAnel( item, qa, WHITE );
        if ( MOSTRAR_RETANGULOS ) {
            DrawRectangleRec( item->ret, Fade( item->cor, 0.5f ) );
            DrawRectangleLines( item->ret.x, item->ret.y, item->ret.width, item->ret.height, BLACK );
        }
    }
}

/**
 * @brief Obtém o quadro de animação atual de um item (anel).
 */
QuadroAnimacao *getQuadroAnimacaoAtualItemAnel( ItemAnel *item ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualItemAnel( item ) );
}

static void desenharQuadroAnimacaoItemAnel( ItemAnel *item, QuadroAnimacao *qa, Color tonalidade ) {

    if ( qa != NULL ) {

        DrawTexturePro(
            rm.texturaItens,
            qa->fonte,
            item->ret,
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULOS ) {
            float xDesenho = item->ret.x + qa->retColisao.x;
            float yDesenho = item->ret.y + qa->retColisao.y;
            DrawRectangle( xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade( GREEN, 0.5f ) );
        }

    }

}

static Animacao *getAnimacaoAtualItemAnel( ItemAnel *item ) {
    return item->animacoes[item->estado];
}