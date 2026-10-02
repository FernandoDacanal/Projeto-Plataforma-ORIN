extern int multiplayer;
extern char ipServidor[256];

#define porta 1337

/**
 * envia uma mensagem com prefixo de 4 bytes indicando o tamanho.
 * retorna o número de bytes enviados (4 + len), ou -1 em erro.
 */
int mp_enviar(int sock, const char *msg);

/**
 * tenta ler do socket e processar todas as mensagens completas
 * disponíveis. deve ser chamada a cada frame, com socket non-blocking.
 *
 * retorna o número de mensagens processadas.
 */
int mp_receber(int sock);

extern int cliente_global;

void mp_notificar_inimigo_morto(int id);
