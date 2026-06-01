#include <stdbool.h>

#include "include/Tipos.h"
#include "include/HUD.h"
#include "include/ResourceManager.h"
#include "include/Utils.h"
#include "include/GameWindow.h"
#include <stdio.h>

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

void desenharTexto(Texture2D fonte, char *string, Rectangle source, Rectangle dest){
    Vector2 posInicial = {dest.x, dest.y};

    int tam = 0;
    int ignorar = 0;
    
    Color cor = BRANCO;

    unsigned char efeito = SEM_EFEITO;

    int tremidax = 0;
	int tremiday = 0;

	int tremida = 0;

    string = unicodeASCII(string);
 
    for(int i = 0; string[i] != '\0'; i++){
        //fazer pulo de linha
        if(string[i] == '\n'){
            dest.x = posInicial.x;
            dest.y += dest.height;
            ++ignorar;
            continue;
        }
        /* não sei pq vc tinha tirado o contrabarra então eu comentei essa parte do código
        if(string[i] == '\\'){
            ++i;
            ++ignorar;
            continue;
        }
        */
        //CORES (Letra Maiuscula)
        if (string[i] == '['){
			switch(string [i + 1]){
				case 'A' : cor = PRETO; ++i; ++ignorar; break;
				case 'B' : cor = ROXO; ++i; ++ignorar; break;
				case 'C' : cor = VERMELHO; ++i; ++ignorar; break;
				case 'D' : cor = LARANJA; ++i; ++ignorar; break;

				case 'E' : cor = AMARELO; ++i; ++ignorar; break;
				case 'F' : cor = VERDECLARO; ++i; ++ignorar; break;
				case 'G' : cor = VERDE; ++i; ++ignorar; break;
				case 'H' : cor = VERDEESCURO; ++i; ++ignorar; break;

				case 'I' : cor = AZULESCURO; ++i; ++ignorar; break;
				case 'J' : cor = AZUL; ++i; ++ignorar; break;
				case 'K' : cor = AZULCLARO; ++i; ++ignorar; break;
				case 'L' : cor = CIANO; ++i; ++ignorar; break;

				case 'M' : cor = BRANCO; ++i; ++ignorar; break;
				case 'N' : cor = CINZACLARO; ++i; ++ignorar; break;
				case 'O' : cor = CINZA; ++i; ++ignorar; break;
				case 'P' : cor = CINZAESCURO; ++i; ++ignorar; break;

                //coloquei rosa para ficar evidente se tiver algum texto formatado errado
				default: cor = PINK; ++i; ++ignorar; break;
			}
			++i;
			++ignorar;
	    }
        else if (string[i] == ']'){
			cor = BRANCO;
			++ignorar;
			continue;
		}
        //FIM CORES
        //EFEITOS(letra minuscula)
        if (string[i] == '{')
		{
			// Efeitos
			if (string[i + 1] == 't')
			{
				efeito = TREMER;
				tremida = 2;
				++i;
				++ignorar;
			}
			else
				string = SEM_EFEITO;
			continue;
		}
        else if (string[i] == '}')
		{
			if (string[i + 1] == 'T')
			{
				efeito = SEM_EFEITO;
				tremidax = 0;
				tremiday = 0;
				++i;
				++ignorar;
			}
			// Pode ter mais de 1 efeito ao mesmo tempo e precisa poder terminar só 1 ou mais
			++ignorar;
			continue;
		}
        if (efeito == TREMER)
		{
			tremidax = tremer(2);
			tremiday = tremer(2);
        }
        
        //desenhar caracteres ascii extendido
        /*
            por algum motivo tem que criar esse unsigned char para os caracteres 
            da tabela ascci extendida funcionarem corretamente.
            eu tentei colocar "if(unsigned char)string[i] >= ' ')" e substituir a 
            variavel c por string[i] só que tava imprimindo os caracteres errados D:
        */
        unsigned char c = string[i];
        if(c >= ' '){
            tam++;
            c -= ' ';	// ' ' == 32

            source.x = (c % 16) * source.width;
            source.y = (c / 16) * source.height;

            DrawTexturePro(
                fonte,
                source,
                (Rectangle){
                    dest.x + tremidax,
                    dest.y + tremiday,
                    dest.width,
                    dest.height
                },
                (Vector2){0},
                0,
                cor
            );
            dest.x += dest.width;
        }
    }
}

void desenharHUD( GameWorld *gw ) {
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
