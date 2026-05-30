/**
 * @file ResourceManager.c
 * @author Prof. Dr. David Buzatto
 * @brief ResourceManager implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "include/raylib/raylib.h"

#include "include/ResourceManager.h"
#include "include/Utils.h"

ResourceManager rm = { 0 };

void loadResourcesResourceManager( void ) {

    rm.texturaJogador = carregarTexturaAlterandoCores( 
        "resources/imagens/sprites/jogador.png",
        (Color[]) {
            { 255, 0, 0, 255 },
            { 0, 255, 0, 255 },
            { 0, 0, 255, 255 }
        },
        (Color[]) {
            BLANK,
            BLANK,
            BLANK
        },
        3
    );

    rm.texturaBadniks = carregarTexturaAlterandoCores( 
        "resources/imagens/sprites/inimigos.png",
        (Color[]) {
            { 255, 0, 0, 255 },
            { 0, 255, 0, 255 },
            { 0, 0, 255, 255 },
        },
        (Color[]) {
            BLANK,
            BLANK,
            BLANK
        },
        3
    );

    rm.texturaItens = carregarTexturaAlterandoCores( 
        "resources/imagens/itens/itens.png",
        (Color[]) {
            { 16, 112, 132, 255 },
        },
        (Color[]) {
            BLANK,
        },
        1
    );

    rm.texturaHUD = carregarTexturaAlterandoCores(
        "resources/imagens/hud/hud-final.png",
        (Color[]){
            {16, 112, 132, 255},
        },
        (Color[]){
            BLANK,
        },
        1
    );

    rm.texturaFonte = LoadTexture( "resources/imagens/tiles/fonte.png" );

    rm.texturaTerreno = LoadTexture( "resources/imagens/tiles/terreno1.png" );
    rm.texturaFundo = LoadTexture( "resources/imagens/fundo/fundo.png" );

    // Filtro de textura nearest-neighbor (ponto) para todas as texturas do jogo.
    // Evita interpolação bilinear nas bordas dos tiles e sprites, que causaria
    // bleeding de cor entre pixels adjacentes — efeito indesejável em pixel art.
    SetTextureFilter( rm.texturaJogador, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaBadniks, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaItens, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaTerreno, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaFundo, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaHUD, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaFonte, TEXTURE_FILTER_POINT );

    rm.somAnel = LoadSound( "resources/sons/efeitos/anel.wav" );
    rm.somFrenagem = LoadSound( "resources/sons/efeitos/frenagem.wav" );
    rm.somHitComAnel = LoadSound( "resources/sons/efeitos/hit-com-anel.wav" );
    rm.somHitInimigo = LoadSound( "resources/sons/efeitos/hit-inimigo.wav" );
    rm.somMorte = LoadSound( "resources/sons/efeitos/morte.wav" );
    rm.somPulo = LoadSound( "resources/sons/efeitos/pulo.wav" );

    rm.musicaFase01 = LoadMusicStream( "resources/sons/musicas/green-hill-zone.mp3" );

}

void unloadResourcesResourceManager( void ) {

    UnloadTexture( rm.texturaJogador );
    UnloadTexture( rm.texturaBadniks );
    UnloadTexture( rm.texturaItens );
    UnloadTexture( rm.texturaTerreno );
    UnloadTexture( rm.texturaFundo );
    UnloadTexture( rm.texturaHUD );
    UnloadTexture( rm.texturaFonte );

    UnloadSound( rm.somAnel );
    UnloadSound( rm.somFrenagem );
    UnloadSound( rm.somHitComAnel );
    UnloadSound( rm.somHitInimigo );
    UnloadSound( rm.somMorte );
    UnloadSound( rm.somPulo );

    UnloadMusicStream( rm.musicaFase01 );

}