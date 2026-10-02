#include "../include/Tipos.h"
#include <string.h>

void chat_adicionar(GameWorld *gw, const char *texto) {
    Chat *c = &gw->chat;

    // desloca para trás se estiver cheio
    if (c->quantidade >= CHAT_MAX_MSGS) {
        for (int i = 1; i < CHAT_MAX_MSGS; i++)
            c->mensagens[i - 1] = c->mensagens[i];
        c->quantidade = CHAT_MAX_MSGS - 1;
    }

    MensagemChat *m = &c->mensagens[c->quantidade++];
    strncpy(m->texto, texto, CHAT_MAX_TAM - 1);
    m->texto[CHAT_MAX_TAM - 1] = '\0';
    m->tempoRestante = CHAT_TEMPO_MSG;
}
