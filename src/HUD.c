#include <stdbool.h>

#include "include/Tipos.h"
#include "include/HUD.h"
#include "include/Utils.h"
#include "include/GameWindow.h"

#include "texto/texto.h"	// Tremer e onda

static Vector2 HUDSuperior = { BORDA, ALTURA_VIRTUAL - TAMANHO_FONTE * 3 - BORDA};
static Vector2 HUDInferior = { TAMANHO_FONTE * 2, ALTURA_VIRTUAL - TAMANHO_FONTE * 3 };

// Desenha a borda quadriculada do jogo
void desenharBorda(Color cor1, Color cor2){
    for(int i = 0; i < LARGURA_VIRTUAL; i++){
        if(i % 2 == 0){
            DrawPixel(i, 0, cor1);
            DrawPixel(i, 1, cor2);

            DrawPixel(i, ALTURA_VIRTUAL - 2, cor1);
            DrawPixel(i, ALTURA_VIRTUAL - 1, cor2);
        } 
        else{
            DrawPixel(i, 0, cor2);
            DrawPixel(i, 1, cor1);

            DrawPixel(i, ALTURA_VIRTUAL - 2, cor2);
            DrawPixel(i, ALTURA_VIRTUAL - 1, cor1);
        }
    }
    for(int i = 0; i < ALTURA_VIRTUAL; i++){
        if(i % 2 == 0){
            DrawPixel(0, i, cor1);
            DrawPixel(1, i, cor2);

            DrawPixel(LARGURA_VIRTUAL - 2,i,  cor1);
            DrawPixel(LARGURA_VIRTUAL - 1,i,  cor2);
        } 
        else{
            DrawPixel(0, i, cor2);
            DrawPixel(1, i, cor1);

            DrawPixel(LARGURA_VIRTUAL - 2,i,  cor2);
            DrawPixel(LARGURA_VIRTUAL - 1,i,  cor1);
        }  
    }
}

// Desenha os elementos da hud
void desenharHUD( GameWorld *gw ) {
    desenharBorda(AZUL, AZULESCURO);
    desenharScore( gw );
    desenharTime( gw );
    desenharRings( gw );
    desenharLives( gw );
}

void desenharScore( GameWorld *gw ) {
    if(gw->jogador->quantidadePontos >= 99999){
        gw->jogador->quantidadePontos = 99999;
    }
    desenharTexto(
        "SCORE",
         HUDSuperior.x, HUDSuperior.y
    );
}
void desenharTime( GameWorld *gw ) {
    int minutos = (int)gw->jogador->quantidadeTempo / 60;

    int dezenaSegundos = (int)gw->jogador->quantidadeTempo % 60 / 10;
    int unidadeSegundos = (int)gw->jogador->quantidadeTempo % 60 % 10;

    int tremor1 = 0;
    int tremor2 = 0;

    if ( gw->jogador->quantidadeAneis == 0 ) {
        tremor1 = efeitoTremer( 2 );
        tremor2 = efeitoTremer( 2 );
    }
    Vector2 pos = {
        HUDSuperior.x + tremor1,
        HUDSuperior.y + TAMANHO_FONTE + tremor2
    };

    desenharTexto(
        "TIME",
        pos.x, pos.y
    );
}

void desenharRings( GameWorld *gw ) {
    if(gw->jogador->quantidadeAneis >= 999){
        gw->jogador->quantidadeAneis = 999;
    }
    if ( gw->jogador->quantidadeAneis != 0 ) {
        desenharTexto(
            ("RINGS "),
           HUDSuperior.x, HUDSuperior.y + TAMANHO_FONTE * 2
        );
    }
    else{
        desenharTexto(
            "{t}RINGS {/t}",
            HUDSuperior.x, HUDSuperior.y + TAMANHO_FONTE * 2
        );
    }
}
void desenharLives( GameWorld *gw ) {

}
