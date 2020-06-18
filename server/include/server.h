/**
 * @file server.h
 * @author Vitor Correa da Silva
 * @date 29 May 2020
 * 
 * @brief Arquivo contendo as estruturas e cabeçalhos de funções do servidor.
 *
 * Esse arquivo contém as definições das funções que são implementadas em server.c.
 * Essas funções tem como objetivo implementar um servidor udp que possibilita o login
 * e gerenciamento de múltiplos jogadores para a realização de partidas de tic-tac-toe (jogo da velha)
 * de forma remota.
 * 
 */

#define TAM_MSG 1024
#define NRO_PARTIDAS_SIMULTANEAS 1000 


#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

/**
 * @brief Endereço Remoto
 * 
 * É utilizado como alias à estrutura sockaddr_in da biblioteca netinet/in.h.
 * Essa estrutura guarda as informações de endereço remoto de outros sockets.
 * 
 * @typedef Sockaddr;
 */
typedef struct sockaddr_in Sockaddr;

/**
 * @brief Mensagem recebida.
 * 
 * ReceivedMessage guarda o conteúdo e informações referentes a uma mensagem recebida via socket UDP.
 * @typedef ReceivedMessage
 */
typedef struct received_message
{
    char data[TAM_MSG]; /**< Conteúdo da mensagem recebida. */
    Sockaddr client_addr; /**< Endereço do remetente da mensagem */
}ReceivedMessage;

/**
 * @brief Jogador conectado
 * 
 * Estrutura que armazena as informações de um jogador conectado ao servidor.
 * 
 * @typedef Player
 */
typedef struct player
{
    unsigned short id; /**< id do jogador dentro de uma partida, podendo ser 1 ou 0. */
    char name[64]; /**< Nome do jogador. */
    Sockaddr addr; /**< Endereço remoto do jogador. */
}Player;

/**
 * @brief Partida de tic-tac-toe
 * 
 * Estrutura que armazena as informações de uma partida de tic-tac-toe que esteja
 * acontecendo no servidor.
 * 
 * @typedef Game
 */
typedef struct game
{
    Player players[2]; /**< Array contendo os jogadores atualmente nessa partida. */
    unsigned short number_of_players; /**< Armazena o número de jogadores conectados a essa partida. */
    int first_player; /**< Armazena o id do jogador que começará/começou jogando essa partida. */
}Game;

/**
 * @brief Cria um socket udp
 *
 * Faz a criação de um socket do tipo UDP e o associa a porta indicada no argumento @p port .
 * 
 * @param port Número da porta UDP que será associada a esse socket, deve ser um valor entre 1 e 65536.
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
int create_socket(int port);

/**
 * @brief Espera pela chegada de uma mensagem em um socket.
 *
 * Essa função inicia a espera de uma mensagem e @b bloqueia o programa nesse estado.
 * O programa só é desbloqueado com a chegada de uma mensagem.
 * 
 * @param sockfd id do descritor do socket que aguardará por uma mensagem.
 * @return Estrutura @c ReceivedMessage contendo informações da mensagem que o socket recebeu.
 * 
 * <b>Exemplo de uso:</b>
 * @code
 * int socket;
 * ReceivedMessage m;
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
ReceivedMessage receive_message(int sockfd);

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
 * @brief Envia uma mensagem para um endereço via socket udp.
 *
 * Função que realiza o envio de uma mensagem de texto via udp.
 * Essa função, diferentemente de @c receive_message , é não bloqueante.
 * O que significa que não há confirmação de envio, ela realiza o envio e segue a execução
 * do programa.
 * É utilizada para a comunicação com os clientes conectados.
 * 
 * @param socket id do descritor do socket que realizará o envio da mensagem.
 * @param msg texto da mensagem a ser enviada.
 * @param client_addr Endereço do cliente que será enviada a mensagem.
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
void send_message(int socket, char *msg, Sockaddr client_addr);

/**
 * @brief Realiza a espera por logins no servidor
 *
 * Essa é a função principal do servidor, fazendo a espera por novas conexões e realizando
 * a alocação de recursos para lidar com os diversos clientes conectados.
 * 
 * Quando um cliente se conecta no servidor, enviando um "LOGIN" para o mesmo, o servidor responde criando
 * uma thread para tratamento dedicado desse cliente, essa lógica está implementada nessa função.
 * 
 * @param socket Socket que receberá o login de clientes.
 * @return Não há retorno.
 * 
 * <b>Exemplo de uso:</b>
 * @code
 * int socket;
 *  
 * socket = create_socket(80);
 * if (socket == -1)
 * {
 *     exit();
 * }
 * 
 * wait_for_login(socket);
 * 
 * @endcode
 * 
 * @warning O socket enviado por parâmetro deve ter sido criado previamente através da função create_socket()
 */
void wait_for_login( int socket );

/**
 * @brief Realiza a inicialização do servidor
 *
 * Essa função é destinada a guardar todas as instruções que devem ser executadas no
 * inicio do programa.
 * Atualmente, ela inicializa o sistema de geração de números aleatórios, para que cada execução
 * do servidor disponha de valores aleatórios diferentes.
 * 
 * @return Não há retorno.
 * 
 */
int init_server(void);


