/**
 * @file Utils.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação das funções utilitárias.
 *
 * @copyright Copyright (c) 2026
 */
#include "include/raylib/raylib.h"
#include <stdlib.h>
#include <string.h>

#include "include/Utils.h"
#include "include/ResourceManager.h"



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
			switch(texto [i + 1]){
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

				default: cor = PINK; ++i; ++ignorar; break;
			}
			++i;
			++ignorar;
		}
		else if (texto[i] == ']')
		{
			cor = WHITE;
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
			tremidax = tremer(2);
			tremiday = tremer(2);
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

char *unicodeASCII(char *string){
    char *stringFormatada = malloc(strlen(string) + 1);
    int posicao = 0;
    for(int i = 0; string[i] != '\0'; i++){
        switch((unsigned char)string[i]){
            case 0xE2:
                switch ((unsigned char)string[i + 1]){
                    case 0x80:
                        switch((unsigned char)string[i + 2]){
                            case 0x93: stringFormatada[posicao++] = (char)0x96; break; //–
                            case 0x94: stringFormatada[posicao++] = (char)0x97; break; //—
                            case 0x98: stringFormatada[posicao++] = (char)0x91; break; //‘
                            case 0x99: stringFormatada[posicao++] = (char)0x92; break; //’
                            case 0x9A: stringFormatada[posicao++] = (char)0x82; break; //‚
                            case 0x9C: stringFormatada[posicao++] = (char)0x93; break; //“
                            case 0x9D: stringFormatada[posicao++] = (char)0x94; break; //”
                            case 0x9E: stringFormatada[posicao++] = (char)0x84; break; //„
                            case 0xA0: stringFormatada[posicao++] = (char)0x86; break; //†
                            case 0xA1: stringFormatada[posicao++] = (char)0x87; break; //‡
                            case 0xA6: stringFormatada[posicao++] = (char)0x85; break; //…
                            case 0xB0: stringFormatada[posicao++] = (char)0x89; break; //‰
                            case 0xB9: stringFormatada[posicao++] = (char)0x8B; break; //‹
                            case 0xBA: stringFormatada[posicao++] = (char)0x9B; break; //›
                        }
                        i++;
                    break;
                    case 0x82:
                        if((unsigned char)string[i + 2] == 0xAC){
                            stringFormatada[posicao++] = (char)0x80; //€
                        }
                        i++;
                    break;
                    case 0x84:
                        if((unsigned char)string[i + 2] == 0xA2){
                            stringFormatada[posicao++] = (char)0x99; //™
                        }
                        i++;
                    break;
                    case 0x88:
                        if ((unsigned char)string[i + 2] == 0x99){
                            stringFormatada[posicao++] = (char)0x95; //∙
                        }
                        i++;
                    break;
                }
                i++;
            break;
            case 0xC2:
                if((unsigned char)string[i + 1] >= 0xA0 && (unsigned char)string[i + 1] <= 0xBF){
                    stringFormatada[posicao++] = (unsigned char)string[i + 1]; //NBSP até ¿
                }
                i++;
            break;
            case 0xC3:
                if((unsigned char)string[i + 1] >= 0x80 && (unsigned char)string[i + 1] <= 0xBF){
                    stringFormatada[posicao++] = (unsigned char)string[i + 1] + 0x40; //à até ÿ
                }
                i++;
            break;
            case 0xC5:
                switch((unsigned char)string[i + 1]){
                    case 0x92: stringFormatada[posicao++] = (char)0x8C; break; //Œ
                    case 0x93: stringFormatada[posicao++] = (char)0x9C; break; //œ
                    case 0xA0: stringFormatada[posicao++] = (char)0x8A; break; //Š
                    case 0xA1: stringFormatada[posicao++] = (char)0x9A; break; //š
                    case 0xB8: stringFormatada[posicao++] = (char)0x9F; break; //Ÿ
                    case 0xBD: stringFormatada[posicao++] = (char)0x8E; break; //Ž
                    case 0xBE: stringFormatada[posicao++] = (char)0x9E; break; //ž
                }
                i++;
            break;

            default: 
                if((unsigned char)string[i] >= 0 && (unsigned char)string[i] <= 127){
                    stringFormatada[posicao++] = string[i];
                }
                else{
                    return "ERRO! CARACTERE INVALIDO";
                }
            break;
        }
    }
    stringFormatada[posicao] = '\0';
    return stringFormatada;
}
