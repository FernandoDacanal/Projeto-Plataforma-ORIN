/**
 * @file Utils.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação das funções utilitárias.
 *
 * @copyright Copyright (c) 2026
 */
#include "include/raylib/raylib.h"

#include "include/Utils.h"
#include "stdlib.h"

/**
 * @brief Carrega uma textura trocando cores.
 */
Texture2D carregarTexturaAlterandoCores( const char *caminhoArquivo, Color *coresFonte, Color *coresAlvo, int quantidadeCores ) {
    Image imagem = LoadImage( caminhoArquivo );
    ImageFormat( &imagem, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 );
    for ( int i = 0; i < quantidadeCores; i++ ) {
        ImageColorReplace( &imagem, coresFonte[i], coresAlvo[i] );
    }
    Texture2D textura = LoadTextureFromImage( imagem );
    UnloadImage( imagem );
    return textura;
}

int tremer(int limite)
{
	// int retorno = rand() % (limite * 2);
	int retorno = rand() % limite;
	//retorno = retorno > limite ? -retorno + limite : retorno;
	int negativo = rand() % 2;
	if (negativo != 0)
		retorno = -retorno;
	// TraceLog(5, "%i", retorno);
	//static int somatorio = 0;
	//somatorio += retorno;
	//TraceLog(5, "%i", somatorio);
	return retorno;
}
