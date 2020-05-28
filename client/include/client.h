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

typedef struct messagem
{
    char data[TAM_MSG];
    struct sockaddr_in client_addr;
}Mensagem;

int create_client_socket(void);
int rand_range(int min, int max);
Mensagem receive_message(int sockfd);
int envia_mensagem(int socketfd, char *msg, char *host, int port);
int login(int socket, char *host, int port);
int play_tictactoe(int socket, char *host, int port);
void clear_stdin(void);
void disconnect(int socket, char *host, int port);