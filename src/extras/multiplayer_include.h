extern int multiplayer;
extern char ipServidor[256];

#define PORTA 1337

/**
 * Envia uma mensagem com prefixo de 4 bytes indicando o tamanho.
 * Retorna o número de bytes enviados (4 + len), ou -1 em erro.
 */
int mp_enviar(int sock, const char *msg);

/**
 * Tenta ler do socket e processar todas as mensagens completas
 * disponíveis. Deve ser chamada a cada frame, com socket non-blocking.
 *
 * Retorna o número de mensagens processadas.
 */
int mp_receber(int sock);
