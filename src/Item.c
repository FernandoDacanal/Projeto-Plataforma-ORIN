/**
 * @file Item.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Item.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "include/ItemAnelVerm.h"
#include "include/raylib/raylib.h"

#include "include/Item.h"
#include "include/ItemAnel.h"
#include "include/Tipos.h"

/**
 * @brief Cria um novo Item.
 */
Item *criarItem( TipoItem tipo ) {

    Item *novoItem = (Item*) malloc( sizeof( Item ) );
    novoItem->objeto = NULL;
    novoItem->tipo = tipo;

    return novoItem;

}

/**
 * @brief Destroi um item.
 */
void destruirItem( Item *item ) {
    if ( item != NULL ) {
        switch ( item->tipo ) {
            case TIPO_ITEM_ANEL:
                destruirItemAnel( (ItemAnel*) item->objeto );
                break;
            case TIPO_ITEM_ANELVERM:
                destruirItemAnelVerm( (ItemAnelVerm*) item->objeto );
                break;
            default:
                break;
        }
        free( item );
    }
}

/**
 * @brief Atualiza um item.
 */
void atualizarItem( Item *item, float delta ) {

    switch ( item->tipo ) {
        case TIPO_ITEM_ANEL:
            atualizarItemAnel( (ItemAnel*) item->objeto, delta );
            break;
        case TIPO_ITEM_ANELVERM:
            atualizarItemAnelVerm( (ItemAnelVerm*) item->objeto, delta );
            break;
        default:
            return;
    }

}

/**
 * @brief Desenha um item.
 */
void desenharItem( Item *item ) {

    switch ( item->tipo ) {
        case TIPO_ITEM_ANEL:
            desenharItemAnel( (ItemAnel*) item->objeto );
            break;
        case TIPO_ITEM_ANELVERM:
            desenharItemAnelVerm( (ItemAnelVerm*) item->objeto );
            break;
        default:
            return;
    }

}