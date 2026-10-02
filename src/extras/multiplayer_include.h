#ifndef MULTIPLAYER_INCLUDE_H
#define MULTIPLAYER_INCLUDE_H

#include "rede.h"

extern int      multiplayer;
extern char     ipServidor[256];
extern socket_t cliente_global;

#define PORTA 1337

int  mp_enviar(socket_t sock, const char *msg);
int  mp_receber(socket_t sock);
void mp_notificar_inimigo_morto(int id);

#endif
