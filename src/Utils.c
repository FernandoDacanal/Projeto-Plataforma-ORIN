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

#include "include/ResourceManager.h"

#include <stdio.h>

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
	if (limite < 2)
		return 0;
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



// enum : unsigned char {
enum {
	SEM_EFEITO, TREMER, NEGRITO, ITALICO
};

void TextoFormatado(char* texto, int posx, int posy, int escala)
{
	int tam = 0;
	Color cor = WHITE;
	unsigned char efeito = SEM_EFEITO;	//TODO: Tem que poder aceitar mais de um efeito ao mesmo tempo
	int ignorar = 0;
	int nova_linha = 0;
	int tremidax = 0;
	int tremiday = 0;

	int tremida = 0;

	// TODO: TEMP
	int coluna = 0;
	while (texto[tam] != '\0') ++tam;
	for (int i = 0; i < tam; i++)
	{
		if (texto[i] == '\\')
		{
			++i;
			++ignorar;
			continue;
		}

		if (texto[i] == '[')
		{
			// Cor
			if (texto[i + 1] == 'R')
			{
				cor = RED;
				++i;
				++ignorar;
			}
			else if (texto[i + 1] == 'G')
			{
				cor = GREEN;
				++i;
				++ignorar;
			}
			else if (texto[i + 1] == 'B')
			{
				cor = BLUE;
				++i;
				++ignorar;
			}
			else
				cor = WHITE;
			++i;
			++ignorar;
		}
		else if (texto[i] == ']')
		{
			cor = WHITE;
			// ++i;
			++ignorar;
			continue;
		}

		if (texto[i] == '{')
		{
			// Efeitos
			if (texto[i + 1] == 'T')
			{
				efeito = TREMER;
				tremida = 2;
				++i;
				++ignorar;
			}
			else
				efeito = SEM_EFEITO;

			// ++i;
			// ++ignorar;
			continue;
		}
		else if (texto[i] == '}')
		{
			if (texto[i + 1] == 'T')
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
		if (texto[i] == '\n')
		{
			++nova_linha;
			coluna = 0;
			// ++i;
			++ignorar;
			continue;
		}

		// int y = ((texto[i] - 32) % 16);
		int y = 0;
		int temp = texto[i] - 32;
		while (temp >= 16)
		{
			temp -= 16;
			++y;
		}
		// DrawTexturePro(rm.texturaFonte, (Rectangle){(texto[i] % 32) * 8, y * 8, 8, 8},
		// 	(Rectangle){posx + ((i - ignorar) * 8) * escala, posy + (nova_linha * 8 * escala), 8 * escala, 8 * escala},
		// 	(Vector2){0}, 0.f, cor);

		if (efeito == TREMER)
		{
			printf("X: %d\n", tremidax = tremer(2));
			printf("Y: %d\n", tremiday = tremer(2));
		}

		DrawTexturePro(
				rm.texturaFonte,
				(Rectangle){(texto[i] % 32) * 8, y * 8, 8, 8},
				(Rectangle){
					posx + (coluna * 8) * escala + tremidax, 
					posy + (nova_linha * 8 * escala) + tremiday,
					8 * escala,
					8 * escala
				},
				(Vector2){0},
				0.f,
				cor
			);

		++coluna;
	}
	// DrawTexturePro(rm.texturaFonte, (Rectangle){('p' - 32) % 16, 5 * 8, 8, 8}, (Rectangle){posx * escala, posy, 8 * escala, 8 * escala}, (Vector2){0}, 0.f, cor);
}
