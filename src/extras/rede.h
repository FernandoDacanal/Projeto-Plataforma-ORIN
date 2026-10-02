#ifndef REDE_H
#define REDE_H

#ifdef _WIN32

    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #include <ws2tcpip.h>

    /* Faz Windows "fingir" que é Linux */

    typedef int socklen_t_win;   /* não precisa, é só ilustrativo */

    #define close(s)            closesocket(s)
    #define errno               WSAGetLastError()
    #define EINTR               WSAEINTR
    #define EWOULDBLOCK         WSAEWOULDBLOCK
    #define EAGAIN              WSAEWOULDBLOCK

    /* fcntl com O_NONBLOCK não existe no Windows.
       Criamos uma versão "fake" que só funciona para o caso
       específico do nosso código (F_GETFL + F_SETFL + O_NONBLOCK) */

    #define F_GETFL             0
    #define F_SETFL             1
    #define O_NONBLOCK          0x0004

    static inline int fcntl(int fd, int cmd, ...) {
        (void)cmd;
        u_long modo = 1;
        return ioctlsocket(fd, FIONBIO, &modo);
    }

    /* WSAStartup automático */

    static inline void __rede_wsa_init(void) {
        static int feito = 0;
        if (!feito) {
            WSADATA wsa;
            WSAStartup(MAKEWORD(2, 2), &wsa);
            feito = 1;
        }
    }

    /* Chama a inicialização na primeira vez que o programa precisar */

    #define socket(...)         (__rede_wsa_init(), socket(__VA_ARGS__))
    #define getaddrinfo(...)    (__rede_wsa_init(), getaddrinfo(__VA_ARGS__))

#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
#endif

#endif
