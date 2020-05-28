#define TAM_MSG 1024
#define NRO_PARTIDAS_SIMULTANEAS 2


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
#include <pthread.h>
/* the next two includes probably aren't relevant for you, but I typically use them all anyway */
#include <math.h>
#include <sys/termios.h>

typedef struct sockaddr_in Sockaddr;

typedef struct messagem
{
    char data[TAM_MSG];
    struct sockaddr_in client_addr;
}Mensagem;

typedef struct player
{
    unsigned short id;
    char name[64];
    Sockaddr addr;
}Player;

typedef struct game
{
    Player players[2];
    unsigned short number_of_players;
    int first_player;
}Game;

int create_socket(int port);
Mensagem receive_message(int sockfd);
int rand_range(int min, int max);
void *client_connection(void *arg);
void send_message(int socket, char *msg, struct sockaddr_in client_addr);
int wait_for_login( int port );
int init_server(void);


