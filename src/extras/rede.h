#ifndef REDE_H
#define REDE_H

#ifdef _WIN32
    typedef unsigned long long socket_t;
    #define REDE_INVALIDO  (~(socket_t)0)
#else
    typedef int socket_t;
    #define REDE_INVALIDO  (-1)
#endif

int  rede_iniciar(void);
void rede_finalizar(void);
int  rede_nonblock(socket_t sock);
int  rede_fechar(socket_t sock);
int  rede_ultimo_erro(void);
int  rede_err_wouldblock(void);
int  rede_err_intr(void);

socket_t rede_criar_servidor(int porta, int backlog);
socket_t rede_aceitar(socket_t servidor);
socket_t rede_conectar(const char *host, int porta);
int      rede_enviar(socket_t sock, const void *dados, int tam);
int      rede_receber(socket_t sock, void *buf, int tam);

#endif

unsigned int rede_htonl(unsigned int v);
unsigned int rede_ntohl(unsigned int v);
