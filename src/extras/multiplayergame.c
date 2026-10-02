#include "../include/Tipos.h"
#include <stdio.h>
#include <string.h>

#include "../include/Mapa.h"

extern GameWorld *gw_global;   // ou passe de outra forma

void mp_processar_mensagem(const char *msg) {
	fprintf(stderr, "RECEBEU: %s\n", msg);   // <-- debug
	if (!gw_global) return;

	GameWorld *gw = gw_global;
	if (!gw) return;

	if (strncmp(msg, "POS ", 4) == 0) {
		float x, y;
		int direcao, estado;
		if (strncmp(msg, "POS ", 4) == 0) {
			float x, y;
			int direcao, estado;
			if (sscanf(msg + 4, "%f %f %d %d", &x, &y, &direcao, &estado) == 4) {
				gw->jogadorRemoto->ret.x = x;
				gw->jogadorRemoto->ret.y = y;
				gw->jogadorRemoto->olhandoParaDireita = (direcao > 0);
				gw->jogadorRemoto->estado = (EstadoJogador) estado;
				gw->remoto.ativo = true;
			}
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
	else if (strncmp(msg, "INIMIGO_MORTO ", 14) == 0) {
		int id;
		if (sscanf(msg + 14, "%d", &id) == 1) {
			matarInimigoPorId(gw->mapa, id);
		}
	}
}
