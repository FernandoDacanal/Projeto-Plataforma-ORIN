#include "../include/Tipos.h"
#include <stdio.h>
#include <string.h>

extern GameWorld *gw_global;   // ou passe de outra forma

void mp_processar_mensagem(const char *msg) {
    GameWorld *gw = gw_global;
    if (!gw) return;

    if (strncmp(msg, "POS ", 4) == 0) {
        float x, y;
        int direcao, estado;
        if (sscanf(msg + 4, "%f %f %d %d", &x, &y, &direcao, &estado) == 4) {
            gw->remoto.x       = x;
            gw->remoto.y       = y;
            gw->remoto.direcao = direcao;
            gw->remoto.estado  = (EstadoJogador) estado;
            gw->remoto.ativo   = true;
        }
    }
    else if (strncmp(msg, "INIMIGO_MORTO ", 14) == 0) {
        int id;
        if (sscanf(msg + 14, "%d", &id) == 1) {
            // marcar o inimigo como morto na sua cópia local
            // (você precisa de uma função para isso no Mapa)
            // mapa_matouInimigo(gw->mapa, id);
        }
    }
    else if (strncmp(msg, "MAPA ", 5) == 0) {
        int n;
        if (sscanf(msg + 5, "%d", &n) == 1) {
            // trocar de mapa
            // MudarFase(gw, n);
            // mapaAtual = n;
        }
    }
    // ... outros tipos depois
}
