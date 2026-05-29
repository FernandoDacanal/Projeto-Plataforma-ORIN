#include <stdio.h>
#include <stdbool.h>

#include "include/Tipos.h"
#include "include/HUD.h"
#include "include/ResourceManager.h"
#include "include/Utils.h"
#include "include/GameWindow.h"

static Vector2 HUDSuperior = { TAMANHO_TILES * 2, TAMANHO_TILES };
static Vector2 HUDInferior = { TAMANHO_TILES * 2, ALTURA_VIRTUAL - TAMANHO_TILES * 3 };

static bool piscar_time = false;
static bool piscar_ring = false;

static float piscar_timeTempo = 0;
static float piscar_ringTempo = 0;

void desenharHUD( GameWorld *gw ) {

    piscarHUD( gw );

    desenharScore( gw );
    desenharTime( gw );
    desenharRings( gw );
    desenharLives( gw );

}

void desenharScore( GameWorld *gw ) {
    desenharSpriteHUD(
        HUD_SCORE_SRC,
        HUDSuperior
    );
    if(gw->jogador->quantidadePontos >= 99999){
        gw->jogador->quantidadePontos = 99999;
    }
    desenharNumero(
        gw->jogador->quantidadePontos * 10,
        (Vector2){
            HUDSuperior.x + TAMANHO_TILES * 6, 
            HUDSuperior.y
        }
    );
}
void desenharTime( GameWorld *gw ) {
    int minutos = (int)gw->jogador->quantidadeTempo / 60;

    int dezenaSegundos = (int)gw->jogador->quantidadeTempo % 60 / 10;
    int unidadeSegundos = (int)gw->jogador->quantidadeTempo % 60 % 10;

    desenharSpriteHUD(
        piscar_time ? HUD_TIME_FLASH_SRC : HUD_TIME_SRC,
        (Vector2){
            HUDSuperior.x, 
            HUDSuperior.y + TAMANHO_TILES * 2
        }
    );

    desenharNumero(
        minutos,
        (Vector2){ 
            HUDSuperior.x + TAMANHO_TILES * 5, 
            HUDSuperior.y + TAMANHO_TILES * 2
        }
    );

    desenharNumero(
        dezenaSegundos,
        (Vector2){
            HUDSuperior.x + TAMANHO_TILES * 7,
            HUDSuperior.y + TAMANHO_TILES * 2
        }
    );

    desenharNumero(
        unidadeSegundos,
        (Vector2){
            HUDSuperior.x + TAMANHO_TILES * 8,
            HUDSuperior.y + TAMANHO_TILES * 2
        }
    );
}

void desenharRings( GameWorld *gw ) {

    int tremor1 = 0;
    int tremor2 = 0;

    if ( gw->jogador->quantidadeAneis == 0 ) {
        tremor1 = tremer( 2 );
        tremor2 = tremer( 2 );
    }

    Vector2 pos = {
        HUDSuperior.x + tremor1,
        HUDSuperior.y + TAMANHO_TILES * 4 + tremor2
    };

    if(gw->jogador->quantidadeAneis >= 999){
        gw->jogador->quantidadeAneis = 999;
    }

    desenharSpriteHUD(
        piscar_ring? HUD_RING_FLASH_SRC : HUD_RING_SRC,
        pos
    );

    desenharNumero(
        gw->jogador->quantidadeAneis,
        (Vector2){
            pos.x + TAMANHO_TILES * 6,
            pos.y
        }
    );

}

// ============================================================================
// LIVES
// ============================================================================

void desenharLives( GameWorld *gw ) {

    desenharSpriteHUD(
        HUD_LIVES_SRC,
        HUDInferior
    );

    desenharNumero(
        gw->jogador->quantidadeVidas,
        (Vector2){
            HUDInferior.x + TAMANHO_TILES * 4,
            HUDInferior.y + TAMANHO_TILES
        }
    );
}


void piscarHUD( GameWorld *gw ) {

    if ( gw->jogador->quantidadeTempo >= TEMPO_LIMITE && gw->jogador->quantidadeTempo - piscar_timeTempo > 0.5f ) {
        piscar_timeTempo = gw->jogador->quantidadeTempo;
        piscar_time = !piscar_time;

    }

    if ( gw->jogador->quantidadeAneis == 0 && gw->jogador->quantidadeTempo - piscar_ringTempo > 0.5f && gw->jogador->quantidadeTempo > 1 ) {
        piscar_ringTempo = gw->jogador->quantidadeTempo;
        piscar_ring = !piscar_ring;

    } else if ( gw->jogador->quantidadeTempo - piscar_ringTempo > 0.5f && gw->jogador->quantidadeTempo > 1 ) {
        piscar_ring = false;

    }

}

void desenharSpriteHUD( Rectangle fonte, Vector2 pos ) {
    DrawTexturePro(
        rm.texturaHUD,
        fonte,
        (Rectangle){
            pos.x,
            pos.y,
            fonte.width,
            fonte.height
        },
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );
}

// ============================================================================
// DESENHO DE NÚMEROS
// ============================================================================

void desenharNumero(int valor, Vector2 pos) {
    int divisor = 1;
    int offset = 0;

    while ( valor / divisor >= 10 ) {
        divisor *= 10;
    }

    while ( divisor > 0 ) {
        int digito = ( valor / divisor ) % 10;
        Rectangle source = {
            digito * TAMANHO_TILES,
            rm.texturaHUD.height - TAMANHO_TILES * 3,
            TAMANHO_TILES,
            TAMANHO_TILES * 2
        };
        desenharSpriteHUD(
            source,
            (Vector2){
                pos.x + offset,
                pos.y
            }
        );
        offset += TAMANHO_TILES;
        divisor /= 10;
    }
}
