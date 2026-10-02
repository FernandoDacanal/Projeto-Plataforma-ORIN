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

//transforma caracteres unicode de uma string no seu equivalente da tabela ascii estendida
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

// Retorna a string
char* mitoa(int num, char* str, int base)
{
	int i = 0;
    bool isNegative = false;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }

    return str;
}

// Retorna o tamanho
int mitoa2(int num, char* str, int base)
{
	int i = 0;
    bool isNegative = false;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return i;
    }

    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }

    return i;
}
