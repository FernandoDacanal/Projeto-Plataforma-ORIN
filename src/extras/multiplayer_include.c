#include "multiplayer_include.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "rede.h"

int  multiplayer    = 0;
char ipServidor[256];
int  cliente_global = -1;

static unsigned char rx_buffer[8192];
static int           rx_tamanho = 0;

// ---------- Envio ----------

int mp_enviar(int sock, const char *msg) {
    int tam_msg = (int) strlen(msg);
    unsigned int tam_rede = htonl((unsigned int) tam_msg);

    unsigned char pacote[4 + 1024];
    if (tam_msg > (int) sizeof(pacote) - 4) {
        return -1;
    }

    memcpy(pacote, &tam_rede, 4);
    memcpy(pacote + 4, msg, tam_msg);

    int total = 4 + tam_msg;
    int enviado = 0;
    while (enviado < total) {
        int n = send(sock, pacote + enviado, total - enviado, 0);
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        enviado += n;
    }
    return total;
}

// ---------- Recepção ----------

static int extrair_mensagem(char *saida, int tam_saida) {
    if (rx_tamanho < 4) return 0;

    unsigned int tam_rede;
    memcpy(&tam_rede, rx_buffer, 4);
    unsigned int tam_msg = ntohl(tam_rede);

    if (tam_msg == 0 || tam_msg >= sizeof(rx_buffer)) {
        return -1;
    }
    if ((int) (4 + tam_msg) > rx_tamanho) {
        return 0;
    }

    int copia = tam_msg < (unsigned int) (tam_saida - 1) ? tam_msg : (tam_saida - 1);
    memcpy(saida, rx_buffer + 4, copia);
    saida[copia] = '\0';

    int restante = rx_tamanho - (4 + tam_msg);
    memmove(rx_buffer, rx_buffer + 4 + tam_msg, restante);
    rx_tamanho = restante;

    return (int) tam_msg;
}

extern void mp_processar_mensagem(const char *msg);

int mp_receber(int sock) {
    while (1) {
        if (rx_tamanho >= (int) sizeof(rx_buffer)) {
            rx_tamanho = 0;
            return -1;
        }

        int n = recv(sock, rx_buffer + rx_tamanho,
                     sizeof(rx_buffer) - rx_tamanho, 0);

        if (n > 0) {
            rx_tamanho += n;
            continue;
        }
        if (n == 0) {
            return -1;
        }
        if (errno == EINTR) continue;
        if (errno == EAGAIN || errno == EWOULDBLOCK) break;
        return -1;
    }

    int processadas = 0;
    while (1) {
        char msg[1024];
        int r = extrair_mensagem(msg, sizeof(msg));
        if (r <= 0) {
            if (r == -1) return -1;
            break;
        }
        mp_processar_mensagem(msg);
        processadas++;
    }
    return processadas;
}

void mp_notificar_inimigo_morto(int id) {
    if (multiplayer == 0 || cliente_global < 0) return;
    char buf[64];
    snprintf(buf, sizeof(buf), "INIMIGO_MORTO %d", id);
    mp_enviar(cliente_global, buf);
}
