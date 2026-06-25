#include "../include/raylib.h"
#include "../include/ResourceManager.h"
#include "../include/Utils.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

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

    bool tremida;
    bool onda;

	bool contorno;

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

// Modifique a função desenharTexto para aceitar argumentos variáveis
void desenharTexto(char *str, int x, int y, ...)
{
    // Primeiro, processamos os argumentos variáveis
    va_list args;
    va_start(args, y); // y é o último parâmetro fixo
    
    // Conta quantos % existem na string
    int numArgs = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '%') {
            numArgs++;
        }
    }
    
    // Cria um array para armazenar os valores inteiros
    int *valores = malloc(numArgs * sizeof(int));
    if (valores != NULL) {
        // Pega todos os argumentos inteiros
        for (int i = 0; i < numArgs; i++) {
            valores[i] = va_arg(args, int);
        }
    }
    
    va_end(args);
    
    // Constrói a string final com as substituições
    char *strFinal = malloc(strlen(str) + numArgs * 12 + 1);
    if (strFinal == NULL) {
        free(valores);
        return;
    }
    
    int idxStrFinal = 0;
    int idxValores = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '%' && valores != NULL && idxValores < numArgs) {
            // Converte o inteiro para string
            char buffer[12];
            sprintf(buffer, "%d", valores[idxValores++]);
            
            // Copia o número para a string final
            for (int j = 0; buffer[j] != '\0'; j++) {
                strFinal[idxStrFinal++] = buffer[j];
            }
        } else {
            strFinal[idxStrFinal++] = str[i];
        }
    }
    strFinal[idxStrFinal] = '\0';
    
    // Processa a string com os valores substituídos
    strFinal = unicodeASCII(strFinal);
    
    // CÓDIGO ORIGINAL DO SEU PARSER AQUI (copiado da sua função desenharTexto)
    EstadoTexto estado = { BRANCO, false, false, false, false, false, 1, 1 };
    Vector2 cursor = { x, y };

    for (int i = 0; strFinal[i] != '\0'; i++)
    {
        if (strFinal[i] == '\\')
        {
            i++;

            if (strFinal[i] == '\0')
                break;
        }

        else if (strFinal[i] == '\n')
        {
            cursor.x = x;
            cursor.y += 8 * estado.escalaY;
            continue;
        }

        else if (strFinal[i] == '[')
        {
            if (strFinal[i + 1] == '/')
            {
                estado.cor = BRANCO;

                while (strFinal[i] != ']')
                    i++;

                continue;
            }

            switch (strFinal[i + 1])
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
                    estado.cor = PINK;
            }

            while (strFinal[i] != ']')
                i++;

            continue;
        }

        else if (strFinal[i] == '{')
        {
            bool desligar = false;

            if (strFinal[i + 1] == '/')
            {
                desligar = true;
                i++;
            }

            switch (strFinal[i + 1])
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

                case 'c':
                case 'C':
                    estado.contorno = !desligar;
                    break;

                case 'x':
                case 'X':
                    if (desligar)
                        estado.escalaX = 1;
                    else
                    {
                        int j;
                        int escala = 0;
                        
                        for (j = i + 2; strFinal[j] >= '0' && strFinal[j] <= '9'; j++)
                        {
                            escala *= 10;
                            escala += strFinal[j] - '0';
                        }
                    
                        if (escala > 0)
                            estado.escalaX = escala;
                    }
                    break;

                case 'y':
                case 'Y':
                    if (desligar)
                        estado.escalaY = 1;
                    else
                    {
                        int j;
                        int escala = 0;
                        
                        for (j = i + 2; strFinal[j] >= '0' && strFinal[j] <= '9'; j++)
                        {
                            escala *= 10;
                            escala += strFinal[j] - '0';
                        }
                        
                        if (escala > 0)
                            estado.escalaY = escala;
                    }
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
                        int j;

                        int escalaX = 0;
                        int escalaY = 0;

                        for (j = i +2; strFinal[j] >= '0' && strFinal[j] <= '9'; j++)
                        {
                            escalaX *= 10;
                            escalaX += strFinal[j] - '0';
                        }

                        if (strFinal[j] == 'x' || strFinal[j] == 'X')
                        {
                            j++;

                            while (strFinal[j] >= '0' && strFinal[j] <= '9')
                            {
                                escalaY *= 10;
                                escalaY += strFinal[j] - '0';
                                j++;
                            }

                            if (escalaX > 0)
                                estado.escalaX = escalaX;

                            if (escalaY > 0)
                                estado.escalaY = escalaY;
                        }
                    }

            }

            while (strFinal[i] != '}')
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
            onda = efeitoOnda(2, i);
        }

        unsigned char c = strFinal[i];

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
                estado.italico ? (estado.contorno ? rm.texturaFonteItalicoContorno : rm.texturaFonteItalico) : (estado.contorno ? rm.texturaFonteContorno : rm.texturaFonte),

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
    
    // Libera a memória alocada
    free(strFinal);
    free(valores);
}

void testeTexto()
{
	DrawRectangle(2, 2, 170, 83, PRETO);
	DrawRectangleLines(2, 2, 171, 84, BRANCO);
	desenharTexto("[C]{x}{y}Normal\n{/y}{/x}"
			"{t}Tremendo{/t}\n"
			"{o}Ondulando{/o}\n"
			"{t}{o}Tremendo e ondulando{/o}{/t}[/]\n"
			"sem formatação\n"
			"[G]{i}{x}{y}Itálico\n{/y}{/x}"
			"{t}Tremendo{/t}\n"
			"{o}Ondulando{/o}\n"
			"{t}{o}Tremendo e ondulando{/t}{/o}{/i}[/]\n"
			"ÁáÉéÍíÓóÚú",
			3, 5);

	// desenharTexto("{i}Abacate{/i}", 5, 5 + 6 * 8);
	// desenharTexto("[A]A{s2x2}b[/]a{/s}{s1x2}cate", 5, 5 + 7 * 8);
	// desenharTexto("[A]A{s2x2}b[/]a{s1x2}cate", 5, 5 + 7 * 8);	// Até pode não fechar os efeitos/cores e só usar outro, mas não recomendo
	// desenharTexto("Normal\n"
	// 		"{x2}x=2{/x}\n"
	// 		"{x3}x=3{/x}\n"
	// 		"{y2}y=2{/y}\n\n"
	// 		"{y3}y=3{/y}\n\n\n"
	// 		"{x2}{y2}x e y = 2{/y}{/x}"
	// 		, 100, 50);
}
