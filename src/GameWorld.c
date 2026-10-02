/**
 * @file GameWorld.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do GameWorld.
 *
 * @copyright Copyright (c) 2026
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/GameWorld.h"
#include "include/Animacao.h"
#include "include/GameWindow.h"
#include "include/Jogador.h"
#include "include/Mapa.h"
#include "include/Tipos.h"
#include "include/ResourceManager.h"
#include "include/HUD.h"
#include "include/Utils.h"

#include "extras/multiplayer_include.h"

#include "include/raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

#include "extras/dev.h"
#include "texto/texto.h"
#include "include/nivel.h"

unsigned char mapaAtual = MAPA1;
static bool velociadeColetada;

static void desenharFundo( GameWorld *gw );
static void atualizarCamera( GameWorld *gw );

static void inicializar( GameWorld *gw );
static void reiniciar( GameWorld *gw );

RenderTexture2D alvoRenderizacao;

bool musica_ativa = true;	// Eu sei que não é legal ficar fazendo variável global ... :)

/**
 * @brief Cria uma instância alocada dinamicamente da struct GameWorld.
 */
GameWorld *createGameWorld( void ) {
    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );
    inicializar( gw );
    return gw;
}

/**
 * @brief Destrói um objeto GameWorld e suas dependências.
 */
void destroyGameWorld( GameWorld *gw ) {
    if ( gw != NULL ) {
        destruirMapa( gw->mapa );
        destruirJogador( gw->jogador );
        destruirJogador( gw->jogadorRemoto );   // <-- novo
        free( gw );
    }
}

/**
 * @brief Lê a entrada do usuário e atualiza o estado do jogo.
 */
void updateGameWorld( GameWorld *gw, float delta ) {
    if(gw->jogador->quantidadeVidas <= 0){
        gw->estadoJogo = gameover;
    }
    atualizarCamera( gw );
    if (musica_ativa)
	{
		if ( !IsMusicStreamPlaying( rm.musicaFase01 ) ) {
			PlayMusicStream( rm.musicaFase01 );
		} else {
			UpdateMusicStream( rm.musicaFase01 );
		}
	}
    // HACK: Temporário de carregar novo mapa
	if (mod_desenvolvedor)
	{
        if (IsKeyPressed(KEY_ZERO))
		{
			MudarFase(gw, MAPA0);
            mapaAtual = MAPA0;
		}
		if (IsKeyPressed(KEY_ONE))
		{
			MudarFase(gw, MAPA1);
            mapaAtual = MAPA1;
		}
		else if (IsKeyPressed(KEY_TWO))
		{
			MudarFase(gw, MAPA2);
            mapaAtual = MAPA2;
		}
	}
    switch(gw->estadoJogo){
        case jogando:
            if (IsKeyPressed(KEY_M))
                musica_ativa = !musica_ativa;
            if (IsKeyPressed( KEY_R ))
            {
                reiniciar( gw );
                return;
            }
			
			// --- Chat ---
			Chat *c = &gw->chat;

			// Atualiza tempo das mensagens
			for (int i = 0; i < c->quantidade; ) {
				c->mensagens[i].tempoRestante -= delta;
				if (c->mensagens[i].tempoRestante <= 0) {
					for (int j = i + 1; j < c->quantidade; j++)
						c->mensagens[j - 1] = c->mensagens[j];
					c->quantidade--;
				} else {
					i++;
				}
			}

			// Input
			if (!c->aberto) {
				if (IsKeyPressed(KEY_T)) {
					c->aberto = true;
					c->tamBuffer = 0;
					c->buffer[0] = '\0';
				}
			} else {
				// Coleta caracteres digitados
				int ch;
				while ((ch = GetCharPressed()) > 0) {
					if (ch >= 32 && ch <= 126 && c->tamBuffer < CHAT_MAX_TAM - 1) {
						c->buffer[c->tamBuffer++] = (char) ch;
						c->buffer[c->tamBuffer]   = '\0';
					}
				}
				if (IsKeyPressed(KEY_BACKSPACE) && c->tamBuffer > 0) {
					c->buffer[--c->tamBuffer] = '\0';
				}
				if (IsKeyPressed(KEY_ENTER) && c->tamBuffer > 0) {
					// Envia para o outro lado
					char pacote[CHAT_MAX_TAM + 8];
					snprintf(pacote, sizeof(pacote), "CHAT %s", c->buffer);
					if (multiplayer != 0 && cliente_global >= 0)
						mp_enviar(cliente_global, pacote);

					// Mostra localmente
					chat_adicionar(gw, c->buffer);

					c->tamBuffer = 0;
					c->buffer[0] = '\0';
					c->aberto    = false;
				}
				if (IsKeyPressed(KEY_ESCAPE)) {
					c->tamBuffer = 0;
					c->buffer[0] = '\0';
					c->aberto    = false;
				}
				// --- Fim chat ---
}
            atualizarMapa( gw->mapa, gw, delta );
            Jogador *j = gw->jogador;
            entradaJogador( j, delta );
            atualizarJogador( j, gw, delta );
			if (gw->remoto.ativo) {
				Animacao *animRemota = gw->jogadorRemoto->animacoes[gw->jogadorRemoto->estado];
				atualizarAnimacao(animRemota, delta);
			}
            if(gw->jogador->estado == ESTADO_JOGADOR_FALANDO){
                gw->estadoJogo = dialogo;
            }
        break;
        case dialogo:
            switch(mapaAtual){
                case MAPA0:
                    if(IsKeyPressed(KEY_SPACE)){
                        gw->estadoJogo = jogando;
                        MudarFase(gw, MAPA1);
                    }
                break;
                case MAPA1:
                    if(IsKeyPressed(KEY_SPACE)){
                        gw->estadoJogo = jogando;
                        MudarFase(gw, MAPA2);
                    }
                break;
                case MAPA2:
                    if(IsKeyPressed(KEY_SPACE)){
                        MudarFase(gw, MAPA0);
                        mapaAtual = MAPA0;
                        gw->estadoJogo = dialogo;
                    }
                break;
            }
            printf("%d", mapaAtual);
        break;
        case gameover:
            gw->jogador->quantidadeVidas = 3;
            MudarFase(gw, MAPA0);
            mapaAtual = MAPA0;
            gw->estadoJogo = dialogo;
        break;
    }
}
/**
 * @brief Desenha o estado do jogo.
 */
void drawGameWorld( GameWorld *gw ) {
	ClearBackground(gw->cor_fundo);

    //elementos alterados pela camera
	BeginMode2D( gw->camera );
	//desenharFundo( gw );
	desenharMapa( gw->mapa );
	desenharJogador( gw->jogador );
		desenharTexto("{o}[j]P1[/]", gw->jogador->ret.x + 9,
				gw->jogador->ret.y - 10);
	
	if (gw->remoto.ativo) {
		desenharTexto("{o}[c]P2[/]", gw->jogadorRemoto->ret.x + 9,
				gw->jogadorRemoto->ret.y - 10);
		desenharJogador( gw->jogadorRemoto );
	}

	EndMode2D();
	// --- Chat ---
	Chat *c = &gw->chat;

	// Histórico (canto inferior esquerdo)
	int baseY = ALTURA_VIRTUAL - 40;
	for (int i = c->quantidade - 1; i >= 0; i--) {
		// fade conforme o tempo acaba
		float alpha = c->mensagens[i].tempoRestante / CHAT_TEMPO_MSG;
		if (alpha > 1.0f) alpha = 1.0f;

		// Sombra + texto (se seu parser suportar cor, use [a] ou algo assim)
		// Aqui uso desenharTexto direto:
		desenharTexto(c->mensagens[i].texto, 10, baseY);

		baseY -= 12;
		if (baseY < 10) break;
	}

	// Barra de input quando o chat está aberto
	if (c->aberto) {
		DrawRectangle(0, ALTURA_VIRTUAL - 20, LARGURA_VIRTUAL, 20, Fade(BLACK, 0.7f));

		char linha[CHAT_MAX_TAM + 4];
		snprintf(linha, sizeof(linha), "> %s_", c->buffer);

		DrawText(linha, 4, ALTURA_VIRTUAL - 14, 10, WHITE);   // <-- DrawText, não desenharTexto
	}
	// --- Fim chat ---


    if(gw->estadoJogo == jogando){
        desenharHUD(gw);
    }
	if (multiplayer == 1)
		desenharTexto("Servidor.", 10, 15);
    
    
    if(gw->estadoJogo == dialogo){
        switch(mapaAtual){
            case MAPA0:
                desenharTexto("  Aperte espaço para começar[/]\n\n\n\n\n\n\n\n{s2x2}[j]SONIC O OURIÇO:[/]\n{/sx}   [e]{o}{i}EM BUSCA DA VELOCIDADE!{/i}{/o}[/][a]", 130, ALTURA_VIRTUAL/2 - 128);
            break;
            case MAPA1:
                DrawRectangle(2, 2, LARGURA_VIRTUAL - 4, 32, PRETO);
                DrawRectangleLines(2, 2, LARGURA_VIRTUAL - 4, 32, BRANCO);
                desenharTexto("{o}\nFASE COMPLETA!{/o}\nAperte espaço para ir ao próximo nível...", 34, 2);
                DrawTexture(rm.texturaPortrait, 2, 2, WHITE);
            break;
            case MAPA2:
                DrawRectangle(2, 2, LARGURA_VIRTUAL - 4, 32, PRETO);
                DrawRectangleLines(2, 2, LARGURA_VIRTUAL - 4, 32, BRANCO);
                desenharTexto("\nObrigado por jogar o nosso jogo!\nVote na nossa equipe, {t}ou então...", 34, 2);
                DrawTexture(rm.texturaPortrait, 2, 2, WHITE);
            break;
            default:
                DrawRectangle(2, 2, LARGURA_VIRTUAL - 4, 32, PRETO);
                DrawRectangleLines(2, 2, LARGURA_VIRTUAL - 4, 32, BRANCO);
                desenharTexto("\nObrigado por jogar o nosso jogo!\nVote na nossa equipe, {t}ou então...", 34, 2);
                DrawTexture(rm.texturaPortrait, 2, 2, WHITE);
            break;
        }
        desenharBorda(AMARELO, VERMELHO);
        
    }
	//DEBUG
	if (mod_desenvolvedor)
	{
		desenharTexto("[a]FPS: %[/]", 5, 104, GetFPS());
		desenharTexto("[a]x: %[/]", 5, 112, (int)gw->jogador->ret.x);
		desenharTexto("[a]y: %[/]", 5, 120, (int)gw->jogador->ret.y);
		if (gw->jogador->invulneravel)
			desenharTexto("[a]Invulnerável: %[/]", 5, 128, gw->jogador->invulneravel);
	}

	if (gw->jogador->acelerado)
		desenharTexto("[c]{t}{c}VELOCIDADE ENCONTRADA!{/c}{/t}[/]", 2, ALTURA_VIRTUAL - 10);

	//testeTexto();
	

}

// TODO: No momento não está sendo utilizado. Remover se não for usar.
// A gente vai usar depois, só tenho que fazer um fundo novo primeiro
static void desenharFundo( GameWorld *gw ) {

    int larguraFundo = rm.texturaFundo.width;
    int larguraMapa = calcularLarguraMapa( gw->mapa );
    int alturaMapa = calcularAlturaMapa( gw->mapa );
    int repeticoes = larguraMapa / larguraFundo;

    int deslocamentoParallax = (int) ( ( gw->camera.target.x / (float) larguraMapa ) * -200 );

    for ( int i = 0; i <= repeticoes; i++ ) {
        DrawTexture( rm.texturaFundo, larguraFundo * i - deslocamentoParallax, alturaMapa - rm.texturaFundo.height, WHITE );
    }
}

static void atualizarCamera( GameWorld *gw ) {

    Jogador *j = gw->jogador;
    Camera2D *c = &gw->camera;

    c->offset.x = (int)(LARGURA_VIRTUAL / 2);
    c->offset.y = (int)(ALTURA_VIRTUAL / 2 - 2);

    // O target é arredondado para o inteiro mais próximo para garantir que a
    // translação da câmera ocorra sempre em posições inteiras de pixel. Sem esse
    // arredondamento, o valor float contínuo de ret.x faz os tiles serem
    // renderizados em posições subpixel, causando frestas visíveis entre eles.
    c->target.x = roundf( j->ret.x + j->ret.width / 2.0f );
    c->target.y = roundf( j->ret.y + j->ret.height / 2.0f );

    int minX = LARGURA_VIRTUAL / 2;
    int maxX = calcularLarguraMapa( gw->mapa ) - LARGURA_VIRTUAL / 2;
    int maxY = calcularAlturaMapa( gw->mapa ) - ALTURA_VIRTUAL / 2 - 16;

    if ( c->target.x < minX ) {
        c->target.x = minX;
    } else if ( c->target.x > maxX ) {
        c->target.x = maxX;
    }

    if ( c->target.y > maxY ) {
        c->target.y = maxY;
    }

}

static void inicializar( GameWorld *gw ) {
	if (mod_desenvolvedor)
		musica_ativa = false;
	
	gw->chat.aberto    = false;
	gw->chat.quantidade = 0;
	gw->chat.tamBuffer  = 0;
	gw->chat.buffer[0]  = '\0';

	gw->cor_fundo = AZULCLARO;
    gw->estadoJogo = jogando;

    gw->mapa = carregarMapa( "resources/mapas/mapa01.txt" );
    // gw->jogador = criarJogador( (float)GetScreenWidth() / 2 + 144, calcularAlturaMapa( gw->mapa ) - 196, 32, 32 );
    gw->jogador = criarJogador( 313, 208, 32, 32 );
    gw->jogador->quantidadeVidas = 0;
	
	gw->jogadorRemoto = criarJogador( 313, 208, 32, 32 );

    gw->camera = (Camera2D) {
        .offset = { 0 },    // deslocamento relativo da câmera em relação ao alvo
        .target = { 0 },    // o alvo da câmera, ou seja, a coordenada em que ela está centralizada
        .rotation = 0.0f,   // rotação da câmera em graus. o pivô é o alvo.
        .zoom = 1.0f        // zoom da câmera. 1.0f significa sem escala
    };
    gw->gravidade = 900;

}

static void reiniciar( GameWorld *gw ) {

    destruirMapa( gw->mapa );
    destruirJogador( gw->jogador );
	destruirJogador( gw->jogadorRemoto );

    if ( IsMusicStreamPlaying( rm.musicaFase01 ) ) {
        StopMusicStream( rm.musicaFase01 );
    }

    inicializar( gw );
}
