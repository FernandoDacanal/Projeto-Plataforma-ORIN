#include <stdbool.h>

#include "include/Tipos.h"
#include "include/HUD.h"
#include "include/Utils.h"
#include "include/GameWindow.h"

#include "include/raylib/raylib.h"
#include "texto/texto.h"	// Tremer e onda

static Vector2 HUD = { BORDA, ALTURA_VIRTUAL - TAMANHO_FONTE * 3 - BORDA - 1 };
static Vector2 HUDInferior = { TAMANHO_FONTE * 2, ALTURA_VIRTUAL - TAMANHO_FONTE * 3 };

// Desenha a borda quadriculada do jogo
void desenharBorda(Color cor1, Color cor2)
{
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

void desenharRet(Color fundo, Color borda)
{
	DrawRectangle(HUD.x - 1, HUD.y - 10, 91, 36, borda);
	DrawRectangle(HUD.x, HUD.y - 9, 89, 34, fundo);
}

// Desenha os elementos da hud
void desenharHUD(GameWorld *gw)
{
    desenharBorda(AZUL, AZULESCURO);
	desenharRet(CINZA, CINZAESCURO);
	
    desenharScore( gw );
    desenharTime( gw );
    desenharRings( gw );
    desenharLives( gw );
}

void desenharScore(GameWorld *gw) {
    if(gw->jogador->quantidadePontos >= 99999)
        gw->jogador->quantidadePontos = 99999;
	
    desenharTexto("{c}Score %{/c}", HUD.x, HUD.y, gw->jogador->quantidadePontos);
}
void desenharTime(GameWorld *gw)
{
    int minutos = (int)gw->jogador->quantidadeTempo / 60;

    int dezenaSegundos = (int)gw->jogador->quantidadeTempo % 60 / 10;
    int unidadeSegundos = (int)gw->jogador->quantidadeTempo % 60 % 10;

    int tremor1 = 0;
    int tremor2 = 0;
	
	Vector2 pos = { HUD.x + tremor1, HUD.y + TAMANHO_FONTE };
	gw->jogador->quantidadeTempo += 2;

	// TODO: Fazer com que o tempo seja diferente para cada mapa.
	if (gw->jogador->quantidadeTempo > 599)	// Temp 10 minutos (5:59)
	{
		if (gw->jogador->quantidadeTempo > 599)
			gw->jogador->quantidadeTempo = 599;

		if ((int)gw->jogador->quantidadeTempo % 60 > 9)
			desenharTexto("[e]{c}{t}Tempo %:%{/t}{/c}[/]", pos.x, pos.y, (int)gw->jogador->quantidadeTempo / 60, (int)gw->jogador->quantidadeTempo % 60);
		else
			desenharTexto("[e]{c}{t}Tempo %:0%{/t}{/c}[/]", pos.x, pos.y, (int)gw->jogador->quantidadeTempo / 60, (int)gw->jogador->quantidadeTempo % 60);
	}
	else
		if ((int)gw->jogador->quantidadeTempo % 60 > 9)
			desenharTexto("[e]{c}Tempo %:%{/c}[/]", pos.x, pos.y, (int)gw->jogador->quantidadeTempo / 60, (int)gw->jogador->quantidadeTempo % 60);
		else
			desenharTexto("[e]{c}Tempo %:0%{/c}[/]", pos.x, pos.y, (int)gw->jogador->quantidadeTempo / 60, (int)gw->jogador->quantidadeTempo % 60);
}

void desenharRings(GameWorld *gw)
{
    if(gw->jogador->quantidadeAneis >= 999){
        gw->jogador->quantidadeAneis = 999;
    }
    if ( gw->jogador->quantidadeAneis != 0 )
		desenharTexto("[e]{c}Anel %{/c}[/]", HUD.x, HUD.y + TAMANHO_FONTE * 2, gw->jogador->quantidadeAneis);
    else
		desenharTexto("[e]{t}{c}Anel 0{/c}{/t}[/]", HUD.x, HUD.y + TAMANHO_FONTE * 2, gw->jogador->quantidadeAneis);
}
void desenharLives(GameWorld *gw)
{
	desenharTexto("[c]{c}Vida %{/c}[/]", HUD.x, HUD.y - 8, gw->jogador->quantidadeVidas);
}
