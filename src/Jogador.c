/**
 * @file Jogador.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Jogador.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "include/raylib/raylib.h"

#include "include/Animacao.h"
#include "include/Inimigo.h"
#include "include/InimigoMotobug.h"
#include "include/InimigoSpikes.h"
#include "include/Item.h"
#include "include/ItemAnel.h"
#include "include/ItemAnelVerm.h"
#include "include/Macros.h"
#include "include/Jogador.h"
#include "include/ResourceManager.h"
#include "include/Tipos.h"

static void desenharQuadroAnimacaoJogador( Jogador *j, QuadroAnimacao *qa, Color tonalidade );
static QuadroAnimacao *getQuadroAnimacaoAtualJogador( Jogador *j );
static Animacao *getAnimacaoAtualJogador( Jogador *j );

static void resolverColisaoJogadorObstaculosMapaX( Jogador *j, Mapa *mapa );
static void resolverColisaoJogadorObstaculosMapaY( Jogador *j, Mapa *mapa );

static void resolverColisaoJogadorItensMapa( Jogador *j, Mapa *mapa );
static void resolverColisaoJogadorInimigosMapa( Jogador *j, Mapa *mapa );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria uma instância alocada dinamicamente da struct Jogador.
 */
Jogador *criarJogador( float x, float y, float w, float h ) {

    Jogador *novoJogador = (Jogador*) malloc( sizeof( Jogador ) );

    novoJogador->ret.x = x;
    novoJogador->ret.y = y;
    novoJogador->ret.width = w;
    novoJogador->ret.height = h;
    novoJogador->vel = (Vector2) { 0 };

    novoJogador->cor = BLUE;

    novoJogador->velAndando = 200;
    novoJogador->velAndandoRapido = 400;
    novoJogador->velCorrendo = 800;
    novoJogador->velPulo = -450;
    novoJogador->velMaxQueda = 600;

    novoJogador->aceleracao = 200;
    novoJogador->desaceleracao = 400;
    novoJogador->frenagem = 1800;

    novoJogador->quantidadePulos = 0;
    novoJogador->quantidadeMaxPulos = 1;

    novoJogador->quantidadeTempo = 0;
    novoJogador->quantidadePontos = 0;
    novoJogador->quantidadeAneis = 0;
    novoJogador->quantidadeVidas = 3;
    novoJogador->quantidadeAneisParaVidas = 100;

    novoJogador->invulneravel = false;
    novoJogador->tempoInvulnerabilidade = 3.0f;
    novoJogador->contadorTempoInvulnerabilidade = 0.0f;

    novoJogador->piscaPisca = false;
    novoJogador->tempoPiscaPisca = 0.05f;
    novoJogador->contadorTempoPiscaPisca = 0.0f;

    novoJogador->freando = false;

    novoJogador->estado = ESTADO_JOGADOR_PARADO;
    novoJogador->olhandoParaDireita = true;

    int quantidadeAnimacoes = 0;

    novoJogador->animacaoParado.quantidadeQuadros = 1;
    novoJogador->animacaoParado.quadroAtual = 0;
    novoJogador->animacaoParado.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoParado.pararNoUltimoQuadro = false;
    novoJogador->animacaoParado.executarUmaVez = false;
    novoJogador->animacaoParado.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoParado, novoJogador->animacaoParado.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoJogador->animacaoParado.quadros,
        novoJogador->animacaoParado.quantidadeQuadros,
        1000,            // duração padrão para todos os quadros
        1, 1,         // início
        32, 32,          // dimensões
        1,               // separação
        false,           // de trás para frente
        (Rectangle) {10, 0, 12, 32}    // retângulo de colisão padrão para cada quadro
    );

    novoJogador->animacaoAndando.quantidadeQuadros = 6;
    novoJogador->animacaoAndando.quadroAtual = 0;
    novoJogador->animacaoAndando.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoAndando.pararNoUltimoQuadro = false;
    novoJogador->animacaoAndando.executarUmaVez = false;
    novoJogador->animacaoAndando.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoAndando, novoJogador->animacaoAndando.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoJogador->animacaoAndando.quadros,
        novoJogador->animacaoAndando.quantidadeQuadros,
        80,              // duração padrão para cada quadro
        1, 34,        // início
        32, 32,          // dimensões
        1,               // separação
        false,           // de trás para frente
        (Rectangle) {10, 0, 12, 32}     // retângulo de colisão padrão para cada quadro (2 * x + largura = tamanho sprite)
    );

    novoJogador->animacaoAndandoRapido.quantidadeQuadros = 6;
    novoJogador->animacaoAndandoRapido.quadroAtual = 0;
    novoJogador->animacaoAndandoRapido.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoAndandoRapido.pararNoUltimoQuadro = false;
    novoJogador->animacaoAndandoRapido.executarUmaVez = false;
    novoJogador->animacaoAndandoRapido.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoAndandoRapido, novoJogador->animacaoAndandoRapido.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoJogador->animacaoAndandoRapido.quadros,
        novoJogador->animacaoAndandoRapido.quantidadeQuadros,
        40,              // duração padrão para cada quadro
        1, 34,        // início
        32, 32,          // dimensões
        1,               // separação
        false,           // de trás para frente
        (Rectangle) {10, 0, 12, 32}    // retângulo de colisão padrão para cada quadro
    );

    novoJogador->animacaoCorrendo.quantidadeQuadros = 4;
    novoJogador->animacaoCorrendo.quadroAtual = 0;
    novoJogador->animacaoCorrendo.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoCorrendo.pararNoUltimoQuadro = false;
    novoJogador->animacaoCorrendo.executarUmaVez = false;
    novoJogador->animacaoCorrendo.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoCorrendo, novoJogador->animacaoCorrendo.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoJogador->animacaoCorrendo.quadros,
        novoJogador->animacaoCorrendo.quantidadeQuadros,
        20,              // duração padrão para cada quadro
        1, 67,         // início
        32, 32,          // dimensões
        1,               // separação
        false,           // de trás para frente
        (Rectangle) {10, 0, 12, 32}    // retângulo de colisão padrão para cada quadro
    );

    novoJogador->animacaoPulando.quantidadeQuadros = 4;
    novoJogador->animacaoPulando.quadroAtual = 0;
    novoJogador->animacaoPulando.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoPulando.pararNoUltimoQuadro = false;
    novoJogador->animacaoPulando.executarUmaVez = false;
    novoJogador->animacaoPulando.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoPulando, novoJogador->animacaoPulando.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoJogador->animacaoPulando.quadros,
        novoJogador->animacaoPulando.quantidadeQuadros,
        40,              // duração padrão para cada quadro
        1, 100,        // início
        32, 32,          // dimensões
        1,               // separação
        false,           // de trás para frente
        (Rectangle) {10, 12, 12, 20}     // retângulo de colisão padrão para cada quadro
    );

    novoJogador->animacaoPulandoRapido.quantidadeQuadros = 4;
    novoJogador->animacaoPulandoRapido.quadroAtual = 0;
    novoJogador->animacaoPulandoRapido.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoPulandoRapido.pararNoUltimoQuadro = false;
    novoJogador->animacaoPulandoRapido.executarUmaVez = false;
    novoJogador->animacaoPulandoRapido.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoPulandoRapido, novoJogador->animacaoPulandoRapido.quantidadeQuadros );
    inicializarQuadrosAnimacao(
        novoJogador->animacaoPulandoRapido.quadros,
        novoJogador->animacaoPulandoRapido.quantidadeQuadros,
        25,              // duração padrão para cada quadro
        1, 100,        // início
        32, 32,          // dimensões
        1,               // separação
        false,           // de trás para frente
        (Rectangle) {10, 12, 12, 20}     // retângulo de colisão padrão para cada quadro
    );

    novoJogador->animacaoPulandoCorrendo.quantidadeQuadros = 4;
    novoJogador->animacaoPulandoCorrendo.quadroAtual = 0;
    novoJogador->animacaoPulandoCorrendo.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoPulandoCorrendo.pararNoUltimoQuadro = false;
    novoJogador->animacaoPulandoCorrendo.executarUmaVez = false;
    novoJogador->animacaoPulandoCorrendo.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoPulandoCorrendo, novoJogador->animacaoPulandoCorrendo.quantidadeQuadros );
    inicializarQuadrosAnimacao(
        novoJogador->animacaoPulandoCorrendo.quadros,
        novoJogador->animacaoPulandoCorrendo.quantidadeQuadros,
        15,              // duração padrão para cada quadro
        1, 100,        // início
        32, 32,          // dimensões
        1,               // separação
        false,           // de trás para frente
        (Rectangle) {10, 12, 12, 20}     // retângulo de colisão padrão para cada quadro
            
    );

    novoJogador->animacoes[ESTADO_JOGADOR_PARADO] = &novoJogador->animacaoParado; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_ANDANDO] = &novoJogador->animacaoAndando; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_ANDANDO_RAPIDO] = &novoJogador->animacaoAndandoRapido; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_CORRENDO] = &novoJogador->animacaoCorrendo; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_PULANDO] = &novoJogador->animacaoPulando; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_PULANDO_RAPIDO] = &novoJogador->animacaoPulandoRapido; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_PULANDO_CORRENDO] = &novoJogador->animacaoPulandoCorrendo; quantidadeAnimacoes++;
    novoJogador->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoJogador;

}

/**
 * @brief Destrói um objeto Jogador e libera seus recursos.
 */
void destruirJogador( Jogador *j ) {
    if ( j != NULL ) {
        for ( int i = 0; i < j->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( j->animacoes[i] );
        }
        free( j );
    }
}

/**
 * @brief Lê a entrada do usuário e atualiza as velocidades do jogador.
 */
void entradaJogador( Jogador *j, float delta ) {

    EstadoJogador estadoAnterior = j->estado;

    if ( IsKeyDown( KEY_RIGHT) || IsKeyDown(KEY_D) ) {
        if ( j->vel.x < 0 ) {
            j->vel.x += j->frenagem * delta;
            if ( !j->freando && j->estado == ESTADO_JOGADOR_CORRENDO ) {
                PlaySound( rm.somFrenagem );
                j->freando = true;
            }
            if ( j->vel.x > 0 ) {
                j->vel.x = 0;
                j->freando = false;
            }
        } else {
            j->vel.x += j->aceleracao * delta;
            if ( j->vel.x > j->velCorrendo ) {
                j->vel.x = j->velCorrendo;
            }
        }
        j->olhandoParaDireita = true;
    } else if ( IsKeyDown( KEY_LEFT) || IsKeyDown(KEY_A) ) {
        if ( j->vel.x > 0 ) {
            j->vel.x -= j->frenagem * delta;
            if ( !j->freando && j->estado == ESTADO_JOGADOR_CORRENDO ) {
                PlaySound( rm.somFrenagem );
                j->freando = true;
            }
            if ( j->vel.x < 0 ) {
                j->vel.x = 0;
                j->freando = false;
            }
        } else {
            j->vel.x -= j->aceleracao * delta;
            if ( j->vel.x < -j->velCorrendo ) {
                j->vel.x = -j->velCorrendo;
            }
        }
        j->olhandoParaDireita = false;
    } else {
        if ( j->vel.x > 0 ) {
            j->vel.x -= j->desaceleracao * delta;
            if ( j->vel.x < 0 ) {
                j->vel.x = 0;
            }
        } else if ( j->vel.x < 0 ) {
            j->vel.x += j->desaceleracao * delta;
            if ( j->vel.x > 0 ) {
                j->vel.x = 0;
            }
        }
    }

    float absVelX = fabsf( j->vel.x );
    if ( j->quantidadePulos > 0 ) {
        if ( absVelX <= j->velAndando ) {
            j->estado = ESTADO_JOGADOR_PULANDO;
        } else if ( absVelX <= j->velAndandoRapido ) {
            j->estado = ESTADO_JOGADOR_PULANDO_RAPIDO;
        } else {
            j->estado = ESTADO_JOGADOR_PULANDO_CORRENDO;
        }
    } else if ( absVelX < 1.0f ) {
        j->estado = ESTADO_JOGADOR_PARADO;
    } else if ( absVelX <= j->velAndando ) {
        j->estado = ESTADO_JOGADOR_ANDANDO;
    } else if ( absVelX <= j->velAndandoRapido ) {
        j->estado = ESTADO_JOGADOR_ANDANDO_RAPIDO;
    } else {
        j->estado = ESTADO_JOGADOR_CORRENDO;
    }

    if ( IsKeyPressed( KEY_SPACE ) && j->quantidadePulos < j->quantidadeMaxPulos ) {
        j->vel.y = j->velPulo;
        j->quantidadePulos++;
        PlaySound( rm.somPulo );
    }

    // sincronização de animações andando e andando rápido
    if ( estadoAnterior == ESTADO_JOGADOR_ANDANDO && j->estado == ESTADO_JOGADOR_ANDANDO_RAPIDO ) {
        sincronizarAnimacao( &j->animacaoAndandoRapido, &j->animacaoAndando );
    } else if ( estadoAnterior == ESTADO_JOGADOR_ANDANDO_RAPIDO && j->estado == ESTADO_JOGADOR_ANDANDO ) {
        sincronizarAnimacao( &j->animacaoAndando, &j->animacaoAndandoRapido );
    }

}

/**
 * @brief Aplica física e resolve colisões do jogador com o mundo.
 */
void atualizarJogador( Jogador *j, GameWorld *gw, float delta ) {
    if(j->quantidadeTempo <= 599){
        j->quantidadeTempo += delta;
    }
    if(j->quantidadeAneis > 0 && j->quantidadeAneis > j->quantidadeAneisParaVidas - 1){
        j->quantidadeVidas++;
        j->quantidadeAneisParaVidas += 100;
    }
    // if(j->quantidadeAneis >= 999){
    //     j->quantidadeAneis = 999;
    // }
    if(j->quantidadeVidas >= 99){
        j->quantidadeVidas = 99;
    }
    // if(j->quantidadePontos >= 99999){
    //     j->quantidadePontos = 99999;
    // }

    
    if ( j->invulneravel ) {

        j->contadorTempoPiscaPisca += delta;
        if ( j->contadorTempoPiscaPisca >= j->tempoPiscaPisca ) {
            j->contadorTempoPiscaPisca = 0.0f;
            j->piscaPisca = !j->piscaPisca;
        }

        j->contadorTempoInvulnerabilidade += delta;
        if ( j->contadorTempoInvulnerabilidade >= j->tempoInvulnerabilidade ) {
            j->contadorTempoInvulnerabilidade = 0.0f;
            j->invulneravel = false;
            j->contadorTempoPiscaPisca = 0.0f;
            j->piscaPisca = false;
        }

    }

    Animacao *animacaoAtual = getAnimacaoAtualJogador( j );
    atualizarAnimacao( animacaoAtual, delta );

    // fase X: move horizontalmente e resolve colisões laterais
    j->ret.x += j->vel.x * delta;
    resolverColisaoJogadorObstaculosMapaX( j, gw->mapa );

    // fase Y: aplica gravidade, move verticalmente e resolve colisões verticais
    j->vel.y += gw->gravidade * delta;
    if ( j->vel.y > j->velMaxQueda ) {
        j->vel.y = j->velMaxQueda;
    }
    j->ret.y += j->vel.y * delta;
    resolverColisaoJogadorObstaculosMapaY( j, gw->mapa );

    resolverColisaoJogadorItensMapa( j, gw->mapa );
    resolverColisaoJogadorInimigosMapa( j, gw->mapa );

}

/**
 * @brief Desenha o jogador.
 */
void desenharJogador( Jogador *j ) {

    if ( !j->piscaPisca ) {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );
        desenharQuadroAnimacaoJogador( j, qa, WHITE );
    }

    if ( MOSTRAR_RETANGULOS ) {
        DrawRectangleRec( j->ret, Fade( j->cor, 0.5f ) );
        DrawRectangleLines( j->ret.x, j->ret.y, j->ret.width, j->ret.height, BLACK );
    }

}

static void desenharQuadroAnimacaoJogador( Jogador *j, QuadroAnimacao *qa, Color tonalidade ) {

    if ( qa != NULL ) {

        DrawTexturePro(
            rm.texturaJogador,
            (Rectangle) {
                qa->fonte.x,
                qa->fonte.y,
                j->olhandoParaDireita ? qa->fonte.width : -qa->fonte.width,
                qa->fonte.height
            },
            j->ret,
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULOS ) {
            float xDesenho = j->olhandoParaDireita
                ? j->ret.x + qa->retColisao.x
                : j->ret.x + j->ret.width - qa->retColisao.x - qa->retColisao.width;
            float yDesenho = j->ret.y + qa->retColisao.y;
            DrawRectangle( xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade( GREEN, 0.5f ) );
        }

    }

}

static QuadroAnimacao *getQuadroAnimacaoAtualJogador( Jogador *j ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualJogador( j ) );
}

static Animacao *getAnimacaoAtualJogador( Jogador *j ) {
    return j->animacoes[j->estado];
}

/**
 * @brief Resolve colisões do jogador com o mapa no eixo X.
 */
static void resolverColisaoJogadorObstaculosMapaX( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->obstaculos;

    while ( el != NULL ) {

        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );

        float deslocamentoX = j->olhandoParaDireita
            ? qa->retColisao.x
            : j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            j->ret.x + deslocamentoX,
            j->ret.y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        Obstaculo *o = (Obstaculo*) el->objeto;

        if ( CheckCollisionRecs( retColCalculado, o->ret ) ) {
            if ( retColCalculado.x + retColCalculado.width / 2 < o->ret.x + o->ret.width / 2 ) {
                j->ret.x = o->ret.x - qa->retColisao.width - deslocamentoX;
            } else {
                j->ret.x = o->ret.x + o->ret.width - deslocamentoX;
            }
            j->vel.x = 0;
        }

        el = el->proximo;

    }

}

/**
 * @brief Resolve colisões do jogador com o mapa no eixo Y.
 */
static void resolverColisaoJogadorObstaculosMapaY( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->obstaculos;

    while ( el != NULL ) {

        Obstaculo *o = (Obstaculo*) el->objeto;
        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );

        float deslocamentoX = j->olhandoParaDireita
            ? qa->retColisao.x
            : j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            j->ret.x + deslocamentoX,
            j->ret.y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        if ( CheckCollisionRecs( retColCalculado, o->ret ) ) {
            if ( retColCalculado.y + retColCalculado.height / 2 < o->ret.y + o->ret.height / 2 ) {
                j->ret.y = o->ret.y - qa->retColisao.height - deslocamentoY;
                j->quantidadePulos = 0;
            } else {
                j->ret.y = o->ret.y + o->ret.height - deslocamentoY;
            }
            j->vel.y = 0;
        }

        el = el->proximo;

    }

}

static void resolverColisaoJogadorItensMapa( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->itens;

    while ( el != NULL ) {

        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );

        float deslocamentoX = j->olhandoParaDireita
            ? qa->retColisao.x
            : j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            j->ret.x + deslocamentoX,
            j->ret.y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        Item *item = (Item*) el->objeto;

        if ( item->tipo == TIPO_ITEM_ANEL ) {

            ItemAnel *itemAnel = (ItemAnel*) item->objeto;

            if ( !itemAnel->ativo || itemAnel->estado == ESTADO_ITEM_ANEL_COLETADO ) {
                el = el->proximo;
                continue;
            }

            QuadroAnimacao *qaItem = getQuadroAnimacaoAtualItemAnel( itemAnel );
            
            Rectangle retColItemCalculado = {
                itemAnel->ret.x + qaItem->retColisao.x,
                itemAnel->ret.y + qaItem->retColisao.y,
                qaItem->retColisao.width,
                qaItem->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColItemCalculado ) ) {
                itemAnel->estado = ESTADO_ITEM_ANEL_COLETADO;
                j->quantidadeAneis++;
                PlaySound( rm.somAnel );
            }

        }

        else if ( item->tipo == TIPO_ITEM_ANELVERM ) {

            ItemAnelVerm *itemAnelVerm = (ItemAnelVerm*) item->objeto;

            if ( !itemAnelVerm->ativo || itemAnelVerm->estado == ESTADO_ITEM_ANELVERM_COLETADO ) {
                el = el->proximo;
                continue;
            }

            QuadroAnimacao *qaItem = getQuadroAnimacaoAtualItemAnelVerm( itemAnelVerm );
            
            Rectangle retColItemCalculado = {
                itemAnelVerm->ret.x + qaItem->retColisao.x,
                itemAnelVerm->ret.y + qaItem->retColisao.y,
                qaItem->retColisao.width,
                qaItem->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColItemCalculado ) ) {
                itemAnelVerm->estado = ESTADO_ITEM_ANELVERM_COLETADO;
                j->quantidadeAneis += 10;
                j->quantidadePontos += 1000;
                PlaySound( rm.somAnel );
            }
        }

        el = el->proximo;

    }

}

static void resolverColisaoJogadorInimigosMapa( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->inimigos;

    while ( el != NULL ) {

        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );

        float deslocamentoX = j->olhandoParaDireita
            ? qa->retColisao.x
            : j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            j->ret.x + deslocamentoX,
            j->ret.y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        Inimigo *inimigo = (Inimigo*) el->objeto;

        QuadroAnimacao *qaInimigo = NULL;
        bool *olhandoParaDireita = NULL;
        Rectangle *ret = NULL;

        if ( inimigo->tipo == TIPO_INIMIGO_MOTOBUG ) {

            InimigoMotobug *motobug = (InimigoMotobug*) inimigo->objeto;

            if ( !motobug->ativo || motobug->estado == ESTADO_INIMIGO_MOTOBUG_MORRENDO ) {
                el = el->proximo;
                continue;
            }

            qaInimigo = getQuadroAnimacaoAtualInimigoMotobug( motobug );
            olhandoParaDireita = &motobug->olhandoParaDireita;
            ret = &motobug->ret;

            float deslocamentoX = *olhandoParaDireita
                ? ret->width - qaInimigo->retColisao.x - qaInimigo->retColisao.width
                : qaInimigo->retColisao.x;
            float deslocamentoY = qaInimigo->retColisao.y;

            Rectangle retColInimigoCalculado = {
                ret->x + deslocamentoX,
                ret->y + deslocamentoY,
                qaInimigo->retColisao.width,
                qaInimigo->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColInimigoCalculado ) ) {

                if ( j->estado >= ESTADO_JOGADOR_PULANDO && j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO ) {
                    j->vel.y = j->velPulo;
                    motobug->estado = ESTADO_INIMIGO_MOTOBUG_MORRENDO;
                    j->quantidadePontos += 10;
                    PlaySound( rm.somHitInimigo );
                } else if ( !j->invulneravel ) {
                    if ( j->quantidadeAneis > 0 ) {
                        j->quantidadeAneis = 0;
                        PlaySound( rm.somHitComAnel );
                    } else if(j->quantidadeVidas > 0) {
                        j->quantidadeVidas--;
                        PlaySound( rm.somMorte );
                    }
                    j->invulneravel = true;
                }

                return; // um inimigo de cada vez!

            }
        }
            
        else if ( inimigo->tipo == TIPO_INIMIGO_SPIKES ) {

            InimigoSpikes *spikes = (InimigoSpikes*) inimigo->objeto;

            if ( !spikes->ativo || spikes->estado == ESTADO_INIMIGO_SPIKES_MORRENDO ) {
                el = el->proximo;
                continue;
            }

            qaInimigo = getQuadroAnimacaoAtualInimigoSpikes( spikes );
            olhandoParaDireita = &spikes->olhandoParaDireita;
            ret = &spikes->ret;

            float deslocamentoX = *olhandoParaDireita
                ? ret->width - qaInimigo->retColisao.x - qaInimigo->retColisao.width
                : qaInimigo->retColisao.x;
            float deslocamentoY = qaInimigo->retColisao.y;

            Rectangle retColInimigoCalculado = {
                ret->x + deslocamentoX,
                ret->y + deslocamentoY,
                qaInimigo->retColisao.width,
                qaInimigo->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColInimigoCalculado ) ) {

                if ( j->estado >= ESTADO_JOGADOR_PULANDO && j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO ) {
                    j->vel.y = j->velPulo;
                    spikes->estado = ESTADO_INIMIGO_SPIKES_MORRENDO;
                    j->quantidadePontos += 10;
                    PlaySound( rm.somHitInimigo );
                } else if ( !j->invulneravel ) {
                    if ( j->quantidadeAneis > 0 ) {
                        j->quantidadeAneis = 0;
                        PlaySound( rm.somHitComAnel );
                    } else if (j->quantidadeVidas > 0) {
                        j->quantidadeVidas--;
                        PlaySound( rm.somMorte );
                    }
                    j->invulneravel = true;
                }

                return; // um inimigo de cada vez!
            }

        }

        el = el->proximo;
    }
}
