#include <client.h>
#include <board.h>

int create_client_socket(void)
{
	int porta;
	struct sockaddr_in clientaddr;
	int socketfd;

	porta = rand_range(20000, 64000);

	// Inicia estrutura de socket e servidor
	bzero((char *) &clientaddr, sizeof(clientaddr));
	clientaddr.sin_family = AF_INET; 
	clientaddr.sin_port = htons(porta); 
	clientaddr.sin_addr.s_addr = INADDR_ANY;

	// Cria socket
	socketfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketfd < 0)
	{
		printf("Error opening socket\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		// Faz o bind
		if(bind(socketfd, (struct sockaddr *) &clientaddr, sizeof(clientaddr)) < 0)
		{ 
				//perror("Falha na nomeação do socket\n");
				exit(EXIT_FAILURE);
		}
	}

	return socketfd;
}

Mensagem receive_message(int sockfd)
{
    Mensagem m;
    unsigned int addr_len;

    addr_len = sizeof(m.client_addr);
    memset(&(m.data), '\0', TAM_MSG);
    recvfrom(sockfd,m.data,TAM_MSG,0,(struct sockaddr *)&(m.client_addr), &addr_len);

    return m;
}

int envia_mensagem(int socketfd, char *msg, char *host, int port)
{
	struct sockaddr_in hostaddr;
	in_addr_t dest_addr = inet_addr(host);

	hostaddr.sin_family = AF_INET; 
    hostaddr.sin_port = htons(port); 
    hostaddr.sin_addr.s_addr = dest_addr;

	sendto(socketfd, (const char *)msg, strlen(msg), 
        MSG_CONFIRM, (const struct sockaddr *) &hostaddr,  
            sizeof(hostaddr));
	
	return 0;
}


int rand_range(int min, int max)
{
	return min + rand()%(max - min + 1);	
}

int login(int socket, char *host, int port)
/* Realiza login no servidor
 * retorna número da porta de comunicação dedicada com o servidor.	 
*/
{
	Mensagem m;
	char msg[6];
	strcpy(msg, "LOGIN");

	envia_mensagem(socket, msg, host, port);
	m = receive_message(socket);

	return atoi(m.data);
}

int play_tictactoe(int socket, char *host, int port)
{
	char msg[1024];
	Mensagem m;
	char gameBoard[9] = {'1','2','3','4','5','6','7','8','9'};
	char playerCharacter = 'O';
	char playerTwoCharacter = 'X';
	int invalid = 0;
	char playerTwoName[64];

	/* envia o nome do jogador para o servidor: */
	printf("Nome do jogador: ");
	/* fgets é alternativa ao gets. fonte: https://stackoverflow.com/questions/43174201/alternative-to-gets */
	if (fgets(msg, sizeof msg, stdin)) {
		msg[strcspn(msg, "\n")] = '\0';
	}
	envia_mensagem(socket, msg, host, port);

	/* Espera pelo estado da partida */
	m = receive_message(socket);

	if (strcmp(m.data, "CHEIO") == 0)
	{
		printf("Já existe uma partida acontecendo. Tente mais tarde.\n");
		sleep(5);
		return 0;
	}

	if (strcmp(m.data, "ESPERANDO") == 0 )
	{
		printf("Conectado. Aguardando outro jogador.\n");
	}

	if (strcmp(m.data, "PRONTO") == 0)
	{
		printf("Partida pronta. O jogo vai começar em breve.\n");
	}

	/* recebe o nome do inimigo */
	m = receive_message(socket);
	strcpy(playerTwoName, m.data);
	
	/* espera pela indicação de que o jogo começou, assim como a ordem dos turnos*/
	m = receive_message(socket);
	printf("%s\n", m.data);

	if(strcmp(m.data, "FIRST") == 0)
	{
		playerCharacter = 'X';
		playerTwoCharacter = 'O';

		do
		{
			DrawBoard(gameBoard);
			if(invalid)
			{
				printf("\njogada invalida");
			}
			
			invalid = 1;

			printf("\nqual sua jogada?");
			if (fgets(msg, sizeof msg, stdin)) {
				msg[strcspn(msg, "\n")] = '\0';
			}

		}while(checkValidPlay(gameBoard, atoi(msg)) == 0);

		gameBoard[atoi(msg) - 1] = playerCharacter;

		envia_mensagem(socket, msg, host, port);
	}

	while (1)
	{
		DrawBoard(gameBoard);

		printf("\naguardando jogada de %s\n", playerTwoName);
		/* espera jogada */
		m = receive_message(socket);
		printf("\njogada: %s\n", m.data);

		gameBoard[atoi(m.data) - 1] = playerTwoCharacter;

		if(CheckPlayerWin(gameBoard) == 1)
		{
			DrawBoard(gameBoard);
			printf("\n%s ganhou\n", playerTwoName);
			printf("desconectando...\n");
			disconnect(socket,host,port);
			envia_mensagem(socket, playerTwoName, host, port);
			sleep(5);

			return 0;
		}
		else if(CheckPlayerWin(gameBoard) == 2)
		{
			DrawBoard(gameBoard);
			printf("Voce e \n%s empataram\n", playerTwoName);
			printf("desconectando...\n");
			disconnect(socket,host,port);
			envia_mensagem(socket, "Empate", host, port);
			sleep(5);
			
			return 0;
		}

		invalid = 0;

		do
		{
			DrawBoard(gameBoard);
			/* Verificar se houve vencedor */
			if(invalid)
			{
				printf("\njogada invalida");
			}
			
			invalid = 1;
			
			printf("\nqual sua jogada?");
			if (fgets(msg, sizeof msg, stdin)) {
				msg[strcspn(msg, "\n")] = '\0';
			}
		}while(checkValidPlay(gameBoard, atoi(msg)) == 0);

		gameBoard[atoi(msg) - 1] = playerCharacter;

		envia_mensagem(socket, msg, host, port);

		if(CheckPlayerWin(gameBoard) == 1)
		{
			DrawBoard(gameBoard);
			printf("\nVoce ganhou de %s\n", playerTwoName);
			printf("desconectando...\n");
			disconnect(socket,host,port);
			sleep(5);
			
			return 0;
		}
		else if(CheckPlayerWin(gameBoard) == 2)
		{
			DrawBoard(gameBoard);
			printf("Voce e \n%s empataram\n", playerTwoName);
			printf("desconectando...\n");
			disconnect(socket,host,port);
			sleep(5);
			
			return 0;
		}
	}

	return 0;
}

void clear_stdin(void)
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void disconnect(int socket, char *host, int port)
{
	char msg[64];
	strcpy(msg,"FIM");
	envia_mensagem(socket, msg, host, port);
}