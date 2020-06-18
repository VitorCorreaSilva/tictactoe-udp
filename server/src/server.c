/**
 * @file server.c
 * @author Vitor Correa da Silva
 * @date 29 May 2020
 * @brief Implementação das funções do servidor
 *
 * 
 */

#include <server.h>
#include <client_thread.h>

/// @cond disabled

int rand_range(int min, int max)
{
	return min + rand()%(max - min + 1);	
}

int create_socket(int port)
{
	int sockfd;
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        return -1;
    }
      
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
    }
	return sockfd;
}

ReceivedMessage receive_message(int sockfd)
{
    ReceivedMessage m;
    unsigned int addr_len;

    addr_len = sizeof(m.client_addr);
    memset(&(m.data), '\0', TAM_MSG);
    recvfrom(sockfd,m.data,TAM_MSG,0,(struct sockaddr *)&(m.client_addr), &addr_len);

    return m;
}

void send_message(int socket, char *msg, struct sockaddr_in client_addr)
{
    sendto(socket, (const char *)msg, strlen(msg), 
        MSG_CONFIRM, (const struct sockaddr *) &client_addr,  
            sizeof(client_addr));
}

void wait_for_login( int socket )
{
    pthread_t client_thread;
    ReceivedMessage m;

    printf("Aguardando a conexão de jogadores.\n");
    while (42)
    {
        m = receive_message(socket);
        if(strcmp(m.data, "LOGIN") == 0)
        {
            printf("Nova conexão.\n");

            /* novo login */
            if(pthread_create(&client_thread, NULL, client_connection_thread, &m.client_addr))
            {
                printf("erro criando thread.\n");
                printf("falha no login,\n");
            }
        }
    }
}

int init_server(void)
{
    srand(time(0));
    return 0;
}
/// @endcond

/**
 * @brief Exibe mensagem de utilização
 * 
 * Exibe a mensagem de utilização:
 * @verbatim usage: ./server <int: port> @endverbatim
 * 
 * Se o nome do binário passado por argumento for "./server", obviamente.
 * @param bin_name Nome do binário que foi executado
 */
void show_usage (char *bin_name)
{
    printf("usage: %s <int: port>\n", bin_name);
}

/**
 * @brief Função main do servidor
 * 
 * Realiza a leitura dos parâmetros de linha de comando, e valida-os.
 * Caso os parâmetros sejam incorretos, exibe a mensagem de utilização chamando
 * a função @c show_usage
 * 
 * Após isso, a função main realiza a inicialização do servidor chamando as funções
 * @c init_server e @c init_shared_variables , cria um socket e inicia a espera por logins através
 * da função @wait_for_login
 * 
 * Caso os parâmetros sejam válidos
 * 
 * @param argc Número de argumentos passados por parâmetro
 * @param argv Array de argumentos do tipo *char
 * @return int 
 */
int main(int argc, char **argv)
{
    int server_port;
    int socket;

    if (argc != 2)
    {
        show_usage(argv[0]);
        return 1;
    }
    
    server_port = atoi(argv[1]);
    if (server_port == 0)
    {
        show_usage(argv[0]);
        return 1;
    }

    if(init_server())
    {
        printf("falha ao inicializar o servidor.\n");
        return 1;
    }
    if (init_shared_variables())
    {
        printf("falha ao inicializar o servidor.\n");
        return 1;
    }

    printf("Iniciando socket de login.\n");
    socket = create_socket(server_port);
    if (socket == -1)
    {
        exit(EXIT_FAILURE);
    }

    printf("iniciando serviço de login.\n");
    wait_for_login(socket);
    close(socket);
    return 0;
}
