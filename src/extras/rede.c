#include "rede.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
#endif

int rede_iniciar(void) {
#ifdef _WIN32
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
#else
    return 0;
#endif
}

void rede_finalizar(void) {
#ifdef _WIN32
    WSACleanup();
#endif
}

int rede_nonblock(socket_t sock) {
#ifdef _WIN32
    u_long modo = 1;
    return ioctlsocket(sock, FIONBIO, &modo);
#else
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags < 0) return -1;
    return fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#endif
}

int rede_fechar(socket_t sock) {
#ifdef _WIN32
    return closesocket(sock);
#else
    return close(sock);
#endif
}

int rede_ultimo_erro(void) {
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

int rede_err_wouldblock(void) {
#ifdef _WIN32
    return WSAEWOULDBLOCK;
#else
    return EWOULDBLOCK;
#endif
}

int rede_err_intr(void) {
#ifdef _WIN32
    return WSAEINTR;
#else
    return EINTR;
#endif
}

socket_t rede_criar_servidor(int porta, int backlog) {
    socket_t s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == REDE_INVALIDO) return REDE_INVALIDO;

    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    struct sockaddr_in info;
    memset(&info, 0, sizeof(info));
    info.sin_family      = AF_INET;
    info.sin_addr.s_addr = INADDR_ANY;
    info.sin_port        = htons(porta);

    if (bind(s, (struct sockaddr*)&info, sizeof(info)) < 0) {
        rede_fechar(s);
        return REDE_INVALIDO;
    }
    if (listen(s, backlog) < 0) {
        rede_fechar(s);
        return REDE_INVALIDO;
    }
    return s;
}

socket_t rede_aceitar(socket_t servidor) {
    return accept(servidor, NULL, NULL);
}

socket_t rede_conectar(const char *host, int porta) {
    struct addrinfo dica, *res, *p;
    char porta_str[16];
    snprintf(porta_str, sizeof(porta_str), "%d", porta);

    memset(&dica, 0, sizeof(dica));
    dica.ai_family   = AF_INET;
    dica.ai_socktype = SOCK_STREAM;
    dica.ai_flags    = 0;

    if (getaddrinfo(host, porta_str, &dica, &res) != 0) return REDE_INVALIDO;

    socket_t sock = REDE_INVALIDO;
    for (p = res; p != NULL; p = p->ai_next) {
        sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock == REDE_INVALIDO) continue;
        if (connect(sock, p->ai_addr, p->ai_addrlen) == 0) break;
        rede_fechar(sock);
        sock = REDE_INVALIDO;
    }
    freeaddrinfo(res);
    return sock;
}

int rede_enviar(socket_t sock, const void *dados, int tam) {
    return send(sock, (const char*)dados, tam, 0);
}

int rede_receber(socket_t sock, void *buf, int tam) {
    return recv(sock, (char*)buf, tam, 0);
}

unsigned int rede_htonl(unsigned int v) {
    return htonl(v);
}

unsigned int rede_ntohl(unsigned int v) {
    return ntohl(v);
}
