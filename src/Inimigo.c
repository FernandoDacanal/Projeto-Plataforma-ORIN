/**
 * @file Inimigo.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Inimigo.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "include/InimigoSpikes.h"
#include "include/raylib/raylib.h"

#include "include/Inimigo.h"
#include "include/InimigoMotobug.h"
#include "include/Tipos.h"

/**
 * @brief Cria um novo Inimigo.
 */
Inimigo *criarInimigo( TipoInimigo tipo ) {

    Inimigo *novoInimigo = (Inimigo*) malloc( sizeof( Inimigo ) );
    novoInimigo->objeto = NULL;
    novoInimigo->tipo = tipo;

    return novoInimigo;

}

/**
 * @brief Destroi um inimigo.
 */
void destruirInimigo( Inimigo *inimigo ) {
    if ( inimigo != NULL ) {
        switch ( inimigo->tipo ) {
            case TIPO_INIMIGO_MOTOBUG:
                destruirInimigoMotobug( (InimigoMotobug*) inimigo->objeto );
                break;
            case TIPO_INIMIGO_SPIKES:
                destruirInimigoSpikes( (InimigoSpikes*) inimigo->objeto );
                break;
            default:
                break;
        }
        free( inimigo );
    }
}

/**
 * @brief Atualiza um inimigo.
 */
void atualizarInimigo( Inimigo *inimigo, GameWorld *gw, float delta ) {

    switch ( inimigo->tipo ) {
        case TIPO_INIMIGO_MOTOBUG:
            atualizarInimigoMotobug( (InimigoMotobug*) inimigo->objeto, gw, delta );
            break;
        case TIPO_INIMIGO_SPIKES:
            atualizarInimigoSpikes( (InimigoSpikes*) inimigo->objeto, gw, delta );
            break;
        default:
            return;
    }

}

/**
 * @brief Desenha um inimigo.
 */
void desenharInimigo( Inimigo *inimigo ) {

    switch ( inimigo->tipo ) {
        case TIPO_INIMIGO_MOTOBUG:
            desenharInimigoMotobug( (InimigoMotobug*) inimigo->objeto );
            break;
        case TIPO_INIMIGO_SPIKES:
            desenharInimigoSpikes( (InimigoSpikes*) inimigo->objeto );
            break;
        default:
            return;
    }

}

/**
 * @brief Resolve colisões do inimigo com o mapa no eixo X.
 */
void resolverColisaoInimigoObstaculosMapaX( Inimigo *inimigo, Mapa *mapa ) {

    ElementoMapa *el = mapa->obstaculos;

    while ( el != NULL ) {

        QuadroAnimacao *qa = NULL;

        bool *olhandoParaDireita = NULL;
        Rectangle *ret = NULL;

        if ( inimigo->tipo == TIPO_INIMIGO_MOTOBUG ) {
            InimigoMotobug *motobug = (InimigoMotobug*) inimigo->objeto;
            qa = getQuadroAnimacaoAtualInimigoMotobug( motobug );
            olhandoParaDireita = &motobug->olhandoParaDireita;
            ret = &motobug->ret;
        }else if (inimigo->tipo == TIPO_INIMIGO_SPIKES){
            InimigoSpikes *spikes = (InimigoSpikes*) inimigo->objeto;
            qa = getQuadroAnimacaoAtualInimigoSpikes( spikes );
            olhandoParaDireita = &spikes->olhandoParaDireita;
            ret = &spikes->ret;
        } else {
            el = el->proximo;
            continue;
        }

        float deslocamentoX = *olhandoParaDireita
            ? ret->width - qa->retColisao.x - qa->retColisao.width
            : qa->retColisao.x;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            ret->x + deslocamentoX,
            ret->y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        Obstaculo *o = (Obstaculo*) el->objeto;

        if ( CheckCollisionRecs( retColCalculado, o->ret ) ) {
            if ( retColCalculado.x + retColCalculado.width / 2 < o->ret.x + o->ret.width / 2 ) {
                ret->x = o->ret.x - qa->retColisao.width - deslocamentoX;
                *olhandoParaDireita = !*olhandoParaDireita;
            } else {
                ret->x = o->ret.x + o->ret.width - deslocamentoX;
                *olhandoParaDireita = !*olhandoParaDireita;
            }
        }

        el = el->proximo;

    }

}

/**
 * @brief Resolve colisões do inimigo com o mapa no eixo Y.
 */
void resolverColisaoInimigoObstaculosMapaY( Inimigo *inimigo, Mapa *mapa ) {

    ElementoMapa *el = mapa->obstaculos;

    while ( el != NULL ) {

        Obstaculo *o = (Obstaculo*) el->objeto;
        QuadroAnimacao *qa = NULL;

        bool *olhandoParaDireita = NULL;
        Rectangle *ret = NULL;
        Vector2 *vel = NULL;

        if ( inimigo->tipo == TIPO_INIMIGO_MOTOBUG ) {
            InimigoMotobug *motobug = (InimigoMotobug*) inimigo->objeto;
            qa = getQuadroAnimacaoAtualInimigoMotobug( motobug );
            olhandoParaDireita = &motobug->olhandoParaDireita;
            ret = &motobug->ret;
            vel = &motobug->vel;
        } else if ( inimigo->tipo == TIPO_INIMIGO_SPIKES ) {
            InimigoSpikes *spikes = (InimigoSpikes*) inimigo->objeto;
            qa = getQuadroAnimacaoAtualInimigoSpikes( spikes );
            olhandoParaDireita = &spikes->olhandoParaDireita;
            ret = &spikes->ret;
            vel = &spikes->vel;
        }else {
            el = el->proximo;
            continue;
        }

        float deslocamentoX = *olhandoParaDireita
            ? ret->width - qa->retColisao.x - qa->retColisao.width
            : qa->retColisao.x;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            ret->x + deslocamentoX,
            ret->y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        if ( CheckCollisionRecs( retColCalculado, o->ret ) ) {
            if ( retColCalculado.y + retColCalculado.height / 2 < o->ret.y + o->ret.height / 2 ) {
                ret->y = o->ret.y - qa->retColisao.height - deslocamentoY;
            } else {
                ret->y = o->ret.y + o->ret.height - deslocamentoY;
            }
            vel->y = 0;
        }

        el = el->proximo;

    }

}