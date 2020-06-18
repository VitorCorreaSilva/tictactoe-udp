/**
 * @file client.h
 * @author Vitor Correa da Silva
 * @date 29 May 2020
 * 
 * @brief Arquivo contendo as estruturas e cabeçalhos de funções do cliente.
 *
 * Esse arquivo contém as definições das funções que são implementadas em client.c.
 * Essas funções tem como objetivo implementar um client udp que faz login no servidor, envia
 * e recebe mensagens.
 * 
 */

#define TAM_MSG 1024
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdarg.h>
#include <time.h>

/* the next two includes probably aren't relevant for you, but I typically use them all anyway */
#include <math.h>
#include <sys/termios.h>
#include <menu.h>


/**
 * @brief Mensagem recebida.
 * 
 * Mensagem guarda o conteúdo e informações referentes a uma mensagem recebida via socket UDP.
 * @typedef Mensagem
 */
typedef struct messagem
{
    char data[TAM_MSG]; /**< Conteúdo da mensagem recebida. */
    struct sockaddr_in client_addr; /**< Endereço do remetente da mensagem */
}Mensagem;

/**
 * @brief Cria um socket udp
 *
 * Faz a criação de um socket do tipo UDP e o associa a porta indicada no argumento @p port .
 * 
 * @param void Não há parametros.
 * @return @c int contendo o id do descritor desse socket. Retorna @b -1 em caso de erro.
 * 
 * <b>Exemplo de uso:</b>
 * @code
 * int socket;
 * socket = create_socket(80);
 * if (socket != -1) {
 *    printf("socket aberto com sucesso.\n");
 * }
 * @endcode
 */
int create_client_socket(void);

/**
 * @brief Gerador de número aleatório dentro de um intervalo
 *
 * Gera um número aleatório dentro do intervalo [ @p min , @p max ]
 * 
 * @param min Valor mínimo do intervalo
 * @param max Valor máximo do intervalo
 * @return @c int Valor aleatório gerado.
 * 
 * <b>Exemplo de uso:</b>
 * @code
 * int nro_aleatorio;
 * 
 * nro_aleatorio = rand_range(1,100)
 * 
 * printf("Valor aleatório entre 1 e 100: %d\n", nro_aleatorio);
 * @endcode
 * 
 */
int rand_range(int min, int max);

/**
 * @brief Espera pela chegada de uma mensagem em um socket.
 *
 * Essa função inicia a espera de uma mensagem.
 * 
 * @param sockfd id do descritor do socket que aguardará por uma mensagem.
 * @return Estrutura @c Mensagem contendo informações da mensagem que o socket recebeu.
 * 
 * <b>Exemplo de uso:</b>
 * @code
 * int socket;
 * Mensagem m;
 * 
 * socket = create_socket(80);
 * if (socket == -1)
 * {
 *     exit();
 * }
 * m = receive_message(socket);
 * 
 * printf("Mensagem recebida: %s\n", m.data);
 * @endcode
 * 
 * @warning O socket enviado por parâmetro deve ter sido criado previamente através da função create_socket()
 */
Mensagem receive_message(int sockfd);

/**
 * @brief Envia uma mensagem para um endereço via socket udp.
 *
 * Função que realiza o envio de uma mensagem de texto via udp.
 * Não há confirmação de envio, ela realiza o envio e segue a execução do programa.
 * É utilizada para a comunicação com o servidor.
 * 
 * @param socketfd id do descritor do socket que realizará o envio da mensagem.
 * @param msg texto da mensagem a ser enviada.
 * @param host Endereço do cliente que será enviada a mensagem.
 * @param host Porta de conexão com o cliente que receberá a mensagem
 * @return Não há retorno.
 * 
 * <b>Exemplo de uso:</b>
 * @code
 * int socket;
 * char msg[1024];
 * ReceivedMessage m;
 *  
 * socket = create_socket(80);
 * if (socket == -1)
 * {
 *     exit();
 * }
 * // espera pelo contato de um cliente.
 * m = receive_message(socket);
 * 
 * // envia "HELLO" de volta para o cliente.
 * strcpy(msg, "HELLO");
 * send_message(socket, msg, m.addr);
 * @endcode
 * 
 * @warning O socket enviado por parâmetro deve ter sido criado previamente através da função create_socket()
 */
int envia_mensagem(int socketfd, char *msg, char *host, int port);

/**
 *@brief Realiza o login no servidor
 * 
 * Envia um pedido de login para o servidor e espera pela porta que será usada para começar o jogo.
 * 
 * @param socket id do descritor do socket que realizará o envio da mensagem.
 * @param host Host de conexao com o servidor 
 * @param port porta de conexão com o servidor
 * @return retorna o numero da porta que sera utilizadao para jogar
 * 
 * 
 */
int login(int socket, char *host, int port);

/**
 * @brief 
 * 
 * Implementa com o uso das outras funções o jogo 
 */
int play_tictactoe(int socket, char *host, int port);

/**
 * @brief Limpa caracteres
 * 
 * Limpa as entradas de teclado desnecessarias que ficam salvas depois que é realizado o scanf 
 * 
 * @param void Não há parametros
 * @return Não há retorno
 * 
 */
void clear_stdin(void);

/**
 * @brief Desconecta do socket
 * 
 * Envia mensagem para o servidor para se desconectar
 * 
 * @param socket id do descritor do socket que realizará o envio da mensagem.
 * @param host Host de conexao com o servidor 
 * @param port porta de conexão com o servidor
 * @return Não há retorno
 * 
 */
void disconnect(int socket, char *host, int port);