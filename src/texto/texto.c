#include "../include/raylib.h"
#include "../include/ResourceManager.h"
#include "../include/Utils.h"

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "texto.h"

#define FONTE_LARGURA 8
#define FONTE_ALTURA  8



Rectangle fonte_tam = { 0, 0, 8, 8 };	// Mude para o tamanho da fonte da textura em questão. Apenas os `width` e `height`, os dois últimos, que importa.
ResourceManager fonte;					// A fonte que deve ser utilizada

typedef struct {
    Color cor;

    bool italico;
	bool negrito;

    int tremida;
    int onda;

    int escalaX;
    int escalaY;
} EstadoTexto;

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

int efeitoOnda(int limite, int i)
{
    if (limite < 1)
        return 0;
    const float velocidade = 60.0f;
    return (int)(sinf(GetTime() * velocidade * 0.15 + i * 0.4f) * limite);
}

void desenharTexto2(char *str, int x, int y)
{
	str = unicodeASCII(str);

	EstadoTexto estado = { BRANCO, false, false, false, false, 1, 1 };
	Vector2 cursor = { x, y };

	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == '\\')
		{
			i++;

			if (str[i] == '\0')
				break;
		}

		else if (str[i] == '\n')
		{
			cursor.x = x;
			cursor.y += 8 * estado.escalaY;
			continue;
		}

		else if (str[i] == '[')
		{
			if (str[i + 1] == '/')
			{
				estado.cor = BRANCO;

				while (str[i] != ']')
					i++;

				continue;
			}

			switch (str[i + 1])
			{
				case 'A':
				case 'a':
					estado.cor = PRETO;
					break;

				case 'B':
				case 'b':
					estado.cor = ROXO;
					break;

				case 'C':
				case 'c':
					estado.cor = VERMELHO;
					break;

				case 'D':
				case 'd':
					estado.cor = LARANJA;
					break;

				case 'E':
				case 'e':
					estado.cor = AMARELO;
					break;

				case 'F':
				case 'f':
					estado.cor = VERDECLARO;
					break;

				case 'G':
				case 'g':
					estado.cor = VERDE;
					break;

				case 'H':
				case 'h':
					estado.cor = VERDEESCURO;
					break;

				case 'I':
				case 'i':
					estado.cor = AZULESCURO;
					break;

				case 'J':
				case 'j':
					estado.cor = AZUL;
					break;

				case 'K':
				case 'k':
					estado.cor = AZULCLARO;
					break;

				case 'L':
				case 'l':
					estado.cor = CIANO;
					break;

				case 'M':
				case 'm':
					estado.cor = BRANCO;
					break;

				case 'N':
				case 'n':
					estado.cor = CINZACLARO;
					break;

				case 'O':
				case 'o':
					estado.cor = CINZA;
					break;

				case 'P':
				case 'p':
					estado.cor = CINZAESCURO;
					break;

				default:
					estado.cor = PINK; // TODO: Temporário para testes.
			}

			while (str[i] != ']')
				i++;

			continue;
		}

		else if (str[i] == '{')
		{
			bool desligar = false;

			if (str[i + 1] == '/')
			{
				desligar = true;
				i++;
			}

			switch (str[i + 1])
			{
				case 'b':
				case 'B':
					estado.negrito = !desligar;
					break;

				case 'i':
				case 'I':
					estado.italico = !desligar;
					break;

				case 'o':
				case 'O':
					estado.onda = !desligar;
					break;

				case 't':
				case 'T':
					estado.tremida = !desligar;
					break;

					// TODO: Também aceitar número
				case 'x':
				case 'X':
					estado.escalaX = desligar ? 1 : 2;
					break;

				case 'y':
				case 'Y':
					estado.escalaY = desligar ? 1 : 2;
					break;

				case 's':
				case 'S':
					if (desligar)
					{
						estado.escalaX = 1;
						estado.escalaY = 1;
					}
					else
					{
						int j = i + 2;

						int escalaX = 0;
						int escalaY = 0;

						while (str[j] >= '0' && str[j] <= '9')
						{
							escalaX *= 10;
							escalaX += str[j] - '0';
							j++;
						}

						if (str[j] == 'x' || str[j] == 'X')
						{
							j++;

							while (str[j] >= '0' && str[j] <= '9')
							{
								escalaY *= 10;
								escalaY += str[j] - '0';
								j++;
							}

							if (escalaX > 0)
								estado.escalaX = escalaX;

							if (escalaY > 0)
								estado.escalaY = escalaY;
						}
					}

			}

			while (str[i] != '}')
				i++;

			continue;
		}

		int tremidaX = 0;
		int tremidaY = 0;

		if (estado.tremida)
		{
			tremidaX = efeitoTremer(2);
			tremidaY = efeitoTremer(2);
		}

		int onda = 0;

		if (estado.onda)
		{
			onda = efeitoOnda(3, i);
		}

		unsigned char c = str[i];

		if (c < ' ')
			continue;

		c -= ' ';

		Rectangle source = { (c % 16) * 8, (c / 16) * 8, fonte_tam.width, fonte_tam.height };

		Rectangle draw =
		{
			cursor.x + tremidaX,
			cursor.y + tremidaY + onda,

			8 * estado.escalaX,
			8 * estado.escalaY
		};

		DrawTexturePro(
				estado.italico ?
				rm.texturaFonteItalico :
				rm.texturaFonte,

				source,
				draw,

				(Vector2){0,0},
				0,
				estado.cor
				);

		if (estado.negrito)
		{
			draw.x++;

			DrawTexturePro(
					estado.italico ?
					rm.texturaFonteItalico :
					rm.texturaFonte,

					source,
					draw,

					(Vector2){0,0},
					0,
					estado.cor
					);
		}

		cursor.x += 8 * estado.escalaX;
	}
}
