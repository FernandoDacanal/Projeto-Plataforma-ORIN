#include <stdbool.h>

#include "include/Tipos.h"
#include "include/HUD.h"
#include "include/ResourceManager.h"
#include "include/Utils.h"
#include "include/GameWindow.h"

static Vector2 HUDSuperior = { BORDA, ALTURA_VIRTUAL - TAMANHO_FONTE * 3 - BORDA};
static Vector2 HUDInferior = { TAMANHO_FONTE * 2, ALTURA_VIRTUAL - TAMANHO_FONTE * 3 };

static bool piscar_time = false;
static bool piscar_ring = false;

static float piscar_timeTempo = 0;
static float piscar_ringTempo = 0;

//fazer função tremer para elementos individuais do texto
//implementar piscar com multiplas cores e usando tint

static void desenharBorda(){
    for(int i = 0; i < LARGURA_VIRTUAL; i++){
        if(i % 2 == 0){
            DrawPixel(i, 0, AZUL);
            DrawPixel(i, 1, AZULESCURO);

            DrawPixel(i, ALTURA_VIRTUAL - 2, AZUL);
            DrawPixel(i, ALTURA_VIRTUAL - 1, AZULESCURO);
        } 
        else{
            DrawPixel(i, 0, AZULESCURO);
            DrawPixel(i, 1, AZUL);

            DrawPixel(i, ALTURA_VIRTUAL - 2, AZULESCURO);
            DrawPixel(i, ALTURA_VIRTUAL - 1, AZUL);
        }  
    }
    for(int i = 0; i < ALTURA_VIRTUAL; i++){
        if(i % 2 == 0){
            DrawPixel(0, i, AZUL);
            DrawPixel(1, i, AZULESCURO);

            DrawPixel(LARGURA_VIRTUAL - 2,i,  AZUL);
            DrawPixel(LARGURA_VIRTUAL - 1,i,  AZULESCURO);
        } 
        else{
            DrawPixel(0, i, AZULESCURO);
            DrawPixel(1, i, AZUL);

            DrawPixel(LARGURA_VIRTUAL - 2,i,  AZULESCURO);
            DrawPixel(LARGURA_VIRTUAL - 1,i,  AZUL);
        }  
    }
}

void desenharTexto(Texture2D textura, char *string, Rectangle source, Rectangle rec){
    Vector2 posInicial = {rec.x, rec.y};
    for(int i = 0; string[i] != '\0'; i++){
        unsigned char c = (unsigned char)string[i];

        //fazer pulo de linha
        if(c == 10){
            rec.x = posInicial.x;
            rec.y += rec.height;
        }
        //desenhar caracteres ascii extendido
        else if(c >= 32){
            c -= 32;
            
            source.x = (c % 16) * source.width;
            source.y = (c / 16) * source.height;

            DrawTexturePro(
                textura,
                source,
                rec,
                (Vector2){0},
                0,
                WHITE
            );
            rec.x += rec.width;
        }
    }
}

void desenharHUD( GameWorld *gw ) {
    /*
    desenharTexto(
        rm.texturaFonte,
        "Modo Debug\nTeste de fontes", 
        (Rectangle){0, 0, 8, 8},
        (Rectangle) {80, 0, 8, 8}
    );
    char tabela[238];
    int pos = 0;
    for (int i = 32; i <= 255; i++) {
        tabela[pos++] = (char)i;
        if ((i - 31) % 16 == 0 && i != 255) {
            tabela[pos++] = '\n';
        }
    }
    tabela[pos] = '\0';

    desenharTexto(
        rm.texturaFonte,
        tabela, 
        (Rectangle){0, 0, 8, 8},
        (Rectangle) {0, 0, 8, 8}
    )
    */
    piscarHUD( gw );
    desenharBorda();
    desenharScore( gw );
    desenharTime( gw );
    desenharRings( gw );
    desenharLives( gw );

}

void desenharScore( GameWorld *gw ) {
    /*
    desenharSpriteHUD(
        HUD_SCORE_SRC,
        HUDSuperior
    );
    */
    desenharTexto(
        rm.texturaFonte,
        "SCORE",
        HUD_FONTE,
        (Rectangle) {HUDSuperior.x, HUDSuperior.y, TAMANHO_FONTE, TAMANHO_FONTE}
    );
    if(gw->jogador->quantidadePontos >= 99999){
        gw->jogador->quantidadePontos = 99999;
    }
    desenharNumero(
        gw->jogador->quantidadePontos * 10,
        (Vector2){
            HUDSuperior.x + TAMANHO_FONTE * 6, 
            HUDSuperior.y
        }
    );
}
void desenharTime( GameWorld *gw ) {
    int minutos = (int)gw->jogador->quantidadeTempo / 60;

    int dezenaSegundos = (int)gw->jogador->quantidadeTempo % 60 / 10;
    int unidadeSegundos = (int)gw->jogador->quantidadeTempo % 60 % 10;

    int tremor1 = 0;
    int tremor2 = 0;

    if ( gw->jogador->quantidadeAneis == 0 ) {
        tremor1 = tremer( 2 );
        tremor2 = tremer( 2 );
    }
    Vector2 pos = {
        HUDSuperior.x + tremor1,
        HUDSuperior.y + TAMANHO_FONTE + tremor2
    };

    desenharTexto(
        rm.texturaFonte,
        "TIME",
        HUD_FONTE,
        (Rectangle) {pos.x, pos.y, TAMANHO_FONTE, TAMANHO_FONTE}
    );

    desenharNumero(
        minutos,
        (Vector2){ 
            HUDSuperior.x + TAMANHO_FONTE * 5, 
            HUDSuperior.y + TAMANHO_FONTE * 2
        }
    );

    desenharNumero(
        dezenaSegundos,
        (Vector2){
            HUDSuperior.x + TAMANHO_FONTE * 7,
            HUDSuperior.y + TAMANHO_FONTE * 2
        }
    );

    desenharNumero(
        unidadeSegundos,
        (Vector2){
            HUDSuperior.x + TAMANHO_FONTE * 8,
            HUDSuperior.y + TAMANHO_FONTE * 2
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
        HUDSuperior.y + TAMANHO_FONTE * 2 + tremor2
    };

    if(gw->jogador->quantidadeAneis >= 999){
        gw->jogador->quantidadeAneis = 999;
    }
    /*
    desenharSpriteHUD(
        piscar_ring? HUD_RING_FLASH_SRC : HUD_RING_SRC,
        pos
    );
    */

    desenharTexto(
        rm.texturaFonte,
        "RINGS",
        HUD_FONTE,
        (Rectangle) {pos.x, pos.y, TAMANHO_FONTE, TAMANHO_FONTE}
    );

    desenharNumero(
        gw->jogador->quantidadeAneis,
        (Vector2){
            pos.x + TAMANHO_FONTE * 6,
            pos.y
        }
    );

}
void desenharLives( GameWorld *gw ) {
    desenharSpriteHUD(
        HUD_LIVES_SRC,
        HUDInferior
    );
    desenharNumero(
        gw->jogador->quantidadeVidas,
        (Vector2){
            HUDInferior.x + TAMANHO_FONTE * 4,
            HUDInferior.y + TAMANHO_FONTE
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
        rm.texturaFonte,
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
            digito * TAMANHO_FONTE,
            TAMANHO_FONTE,
            TAMANHO_FONTE,
            TAMANHO_FONTE
        };
        desenharSpriteHUD(
            source,
            (Vector2){
                pos.x + offset,
                pos.y
            }
        );
        offset += TAMANHO_FONTE;
        divisor /= 10;
    }
}
