/**
 * @file client_thread.c
 * @author Vitor Correa da Silva
 * @date 29 May 2020
 * @brief Implementação das funções de conexão via thread dedicada.
 *
 * 
 */

#include <server.h>
#include <client_thread.h>

/* Recursos que serão compartilhados entre threads */
Game tictactoe[NRO_PARTIDAS_SIMULTANEAS];

/* Mutex para fazer a proteção dos recursos */
pthread_mutex_t lock;

/// @cond disabled

int init_shared_variables(void)
/*
    Função para inicializar todos os recursos compartilhados.
*/
{
    int i;
    if (pthread_mutex_init(&lock, NULL) != 0) { 
        printf("Inicialização do mutex falhou.\n"); 
        return 1;
    }
    pthread_mutex_lock(&lock);
    for (i = 0; i < NRO_PARTIDAS_SIMULTANEAS; i++)
    {
        tictactoe[i].number_of_players = 0;
    }
    pthread_mutex_unlock(&lock);
    return 0;
}

void leave_game(int game_id)
{
    pthread_mutex_lock(&lock);
    tictactoe[game_id].number_of_players--;
    pthread_mutex_unlock(&lock);
}

void start_remote_tictactoe_game(int socket, Player p, unsigned short game_id)
{
    ReceivedMessage m;
    int outro_jogador = 1 - p.id;

    while ( strcmp(m.data, "FIM"))
    {
        /* Espera jogada */
        m = receive_message(socket);

        /* Envia jogada para o outro jogador */
        send_message(socket, m.data, tictactoe[game_id].players[outro_jogador].addr);
    }

    m = receive_message(socket);
    write_in_file_ranking(m.data);

    leave_game(game_id);
    printf("Jogador %s saiu.\n", p.name);
    printf("Finalizando partida %i\n", game_id);

    return;
}

int search_for_available_game( void )
{
    int i = 0;
    /* Procura por alguma sala com alguém sozinho esperando */
    while(i < NRO_PARTIDAS_SIMULTANEAS && tictactoe[i].number_of_players != 1)
    {
        i++;
    }
    if (i == NRO_PARTIDAS_SIMULTANEAS)
    {
        /* Não encontrou. Procura por uma sala vazia. */
        i = 0;
        while(tictactoe[i].number_of_players == 2)
        /* Percorre o array de partidas procurando um com menos de 2 jogadores */
        {
            i++;
            if (i == NRO_PARTIDAS_SIMULTANEAS)
            {
                /* Não encontrou uma partida */
                return -1;
            }
        }
    }
    return i;
}

int insert_player_in_game(Player p, int game_id)
{
    p.id = tictactoe[game_id].number_of_players;
    tictactoe[game_id].players[tictactoe[game_id].number_of_players] = p;
    tictactoe[game_id].number_of_players++;
    return p.id;
}

void wait_all_players_to_connect(int game_id)
{
    while (tictactoe[game_id].number_of_players < 2)
    {
        sleep(1);
    }
}

void send_file_ranking()
{

}

void write_in_file_ranking(char *msg){
    FILE *arq;
    int result;
    arq = fopen("ranking.txt", "a+"); // Cria um arquivo texto para gravação
    if (arq == NULL) // Se não conseguiu criar
    {
        printf("Problemas na CRIACAO do arquivo\n");
        return;
    }
    result = fprintf(arq,"%s - %i\n",msg, 1); 
    if (result == EOF)
    {
        printf("Erro na Gravacao\n");
    }
    fclose(arq);
}

void *client_connection_thread(void *client_address)
{
    ReceivedMessage m;
    int socket;
    unsigned short i;
    int game_id;
    Player p;
    char msg[2056];
    int client_port;
    Sockaddr *address_rcv = (Sockaddr *)client_address;
    int outro_jogador;

    /* Associa o endereço recebido por endereço ao endereço desse jogador */
    p.addr = *address_rcv;

    /* cria novo socket para comunicação dedicada com esse cliente */
    /* Sorteia uma porta e abre um socket com bind nessa porta */
    /* Por envolver sorteio, em caso de repetição realiza o procedimento até 3 vezes */
    i = 0;
    do
    {
        client_port = rand_range(20000,64000);
        socket = create_socket(client_port);
        i++;
    }while(socket == -1 && i < 10);

    if (socket == -1)
    {
        printf("Erro ao alocar porta para o cliente, finalizando conexão.");
        strcpy(msg, "ERRO");
        return NULL;
    }
    /* armazena a porta como string, para ser enviada ao cliente */
    sprintf(msg,"%d", client_port);
    
    /* envia a porta para o cliente */
    send_message(socket, msg, p.addr);

    /* Espera pelo nome do cliente */
    m = receive_message(socket);

    /* O nome recebido é associado como nome do jogador */
    strcpy(p.name, m.data);

    
    printf("Jogador %s conectado.\n", p.name);
    printf("%s: Procurando jogo.\n", p.name);

    /*
    *   Busca por uma sala de jogo no servidor.
    *   Nesse ponto há 3 possibilidades:
    *   - ESPERANDO: O jogador encontrou uma sala vazia e está esperando outro jogador.
    *   - PRONTO: Entrou em uma sala com outro jogador.
    *   - CHEIO: Todas salas de jogo estão cheias
    */
    pthread_mutex_lock(&lock);
    game_id = search_for_available_game();

    if (game_id == -1)
    {
        pthread_mutex_unlock(&lock);
        printf("%s: Todas salas estão cheias, desconectando.\n", p.name);
        strcpy(msg, "CHEIO");
        send_message(socket, msg, p.addr);
        close(socket);
        return NULL;
    }

    /* Encontrou uma partida com espaço livre, a partida tem indice igual a game_id*/
    /* insere o jogador na partida */
    p.id = insert_player_in_game(p, game_id);
    pthread_mutex_unlock(&lock);
    outro_jogador = 1 - p.id;

    /* envia o estado atual para o jogador */
    strcpy(msg, (tictactoe[game_id].number_of_players == 1) ? "ESPERANDO" : "PRONTO");
    printf("%s: Partida de número %d localizada. Estado: %s\n", p.name, game_id, msg);
    send_message(socket, msg, p.addr);

    /* se o estado for PRONTO, faz o sorteio dos turnos */
    if (strcmp(msg, "PRONTO") == 0)
    {
        pthread_mutex_lock(&lock);
        tictactoe[game_id].first_player = rand_range(0,1);
        pthread_mutex_unlock(&lock);
    }

    /* Se houver apenas um jogador conectado, espera o outro conectar */
    wait_all_players_to_connect(game_id);

    /* envia seu nome para o advversario */
    strcpy(msg, p.name);
    send_message(socket, msg, tictactoe[game_id].players[outro_jogador].addr);
    sleep(1);

    /* Jogo começou - envia FIRST para o primeiro jogador, e SECOND para o segundo.*/
    printf("%s: partida de número %i iniciada.\n", p.name, game_id);
    if (p.id == tictactoe[game_id].first_player)
    {
        strcpy(msg, "FIRST");
    }
    else
    {
        strcpy(msg, "SECOND");
    }
    send_message(socket, msg, p.addr);

    /* Feito isso, o jogo irá começar entre os dois jogadores conectados.*/
    start_remote_tictactoe_game(socket, p, game_id);

    close(socket);
    return NULL;
}
/// @endcond
