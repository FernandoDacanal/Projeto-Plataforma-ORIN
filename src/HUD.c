#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "include/Tipos.h"
#include "include/HUD.h"
#include "include/ResourceManager.h"
#include "include/Utils.h"
#include "include/GameWindow.h"

static Vector2 HUDSuperior = { BORDA, ALTURA_VIRTUAL - TAMANHO_FONTE * 3 - BORDA};
static Vector2 HUDInferior = { TAMANHO_FONTE * 2, ALTURA_VIRTUAL - TAMANHO_FONTE * 3 };

//função responsável por fazer o efeito de tremer o texto
int efeitoTremer(int limite)
{
	if (limite < 2)
		return 0;
	int retorno = rand() % limite;
	int negativo = rand() % 2;
	if (negativo != 0)
		retorno = -retorno;
	return retorno;
}
//função responsável por fazer o efeito de onda no texto
int efeitoOnda(int limite, int i)
{
    if (limite < 1)
        return 0;
    const float velocidade = 60.0f;
    return (int)(sinf(GetTime() * velocidade * 0.15 + i * 0.4f) * limite);
}

//desenha a borda quadriculada do jogo
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

void desenharTexto(char *string, Rectangle source, Rectangle dest){
    Vector2 posInicial = {dest.x, dest.y};

    int tam = 0;
    int ignorar = 0;
    
    Color cor = BRANCO;
    unsigned char efeito = SEM_EFEITO;
    int tremida = 0;
    int onda = 0;
    int larguraFonte = 1;
    int alturaFonte = 1;
    bool italico = false;

    string = unicodeASCII(string);
 
    for(int i = 0; string[i] != '\0'; i++){
        //fazer pulo de linha
        if(string[i] == '\n'){
            dest.x = posInicial.x;
            dest.y += dest.height * alturaFonte;
            ++ignorar;
            continue;
        }
        //não sei pq vc tinha tirado o contrabarra então eu comentei essa parte do código
        /*
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
        //EFEITOS(letra minuscula) - definições
        if (string[i] == '{')
		{
            switch(string[i + 1]){
                case 't':
                    efeito = TREMER; ++i; ++ignorar; break;
                case 'o':
                    efeito = ONDA; ++i; ++ignorar; break;
                case 'i':
                    efeito = ITALICO; ++i; ++ignorar; break;
                case 'x': 
                    efeito = AUMENTARX; ++larguraFonte; ++i; ++ignorar; break;
                case 'y': 
                    efeito = AUMENTARY; ++alturaFonte; ++i; ++ignorar; break;
                default: efeito = SEM_EFEITO; ++i; ++ignorar; break;
            }
            ++i;
            ++ignorar;
		}
        else if (string[i] == '}'){
			efeito = SEM_EFEITO;
			++ignorar;
			continue;
		}
        //implementação
        switch(efeito){
            case TREMER: tremida = efeitoTremer(2); break;
            case ITALICO: italico = true; break;
            case ONDA: onda = efeitoOnda(2, i); break;
            case AUMENTARX: break;
            case AUMENTARY: break;
            case SEM_EFEITO:
                italico = false;
                tremida = 0;
                onda = 0;
                larguraFonte = 1;
                alturaFonte = 1;
            break;
        }
        //FIM EFEITOS

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
                italico ?  rm.texturaFonteItalico : rm.texturaFonte,
                source,
                (Rectangle){
                    dest.x + tremida,
                    dest.y + tremida + onda,
                    dest.width * larguraFonte,
                    dest.height * alturaFonte
                },
                (Vector2){0},
                0,
                cor
            );
            dest.x += dest.width * larguraFonte;
        }
    }
}
//desenha os elementos da hud
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
        HUD_FONTE,
        (Rectangle) {HUDSuperior.x, HUDSuperior.y, TAMANHO_FONTE, TAMANHO_FONTE}
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
        HUD_FONTE,
        (Rectangle) {pos.x, pos.y, TAMANHO_FONTE, TAMANHO_FONTE}
    );
}

void desenharRings( GameWorld *gw ) {
    if(gw->jogador->quantidadeAneis >= 999){
        gw->jogador->quantidadeAneis = 999;
    }
    if ( gw->jogador->quantidadeAneis != 0 ) {
        desenharTexto(
            ("RINGS "),
            HUD_FONTE,
            (Rectangle) {HUDSuperior.x, HUDSuperior.y + TAMANHO_FONTE * 2, TAMANHO_FONTE, TAMANHO_FONTE}
        );
    }
    else{
        desenharTexto(
            "{tRINGS ",
            HUD_FONTE,
            (Rectangle) {HUDSuperior.x, HUDSuperior.y + TAMANHO_FONTE * 2, TAMANHO_FONTE, TAMANHO_FONTE}
        );
    }
}
void desenharLives( GameWorld *gw ) {

}
