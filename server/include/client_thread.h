/**
 * @file client_thread.h
 * @author Vitor Correa da Silva
 * @date 29 May 2020
 * 
 * @brief Arquivo contendo cabeçalhos de funções que implementam as interações do servidor com os clientes conectados.
 *
 * Esse arquivo contém as definições das funções que são implementadas em client_thread.c.
 * Essas funções tem como objetivo implementar o funcionamento da troca de mensagens entre servidor e clientes quando
 * estes já estão conectados.
 * 
 */
#define TAM_JOGADA 1089

/**
 * @brief Inicializa as variáveis compartilhadas.
 *
 * Essa função inicializa as variáveis que são compartilhadas entre as threads
 * de conexão com os clientes.
 * Inicializa os seguintes recursos:
 * - pthread_mutex_t lock
 * - Game tictactoe[NRO_PARTIDAS_SIMULTANEAS]
 * 
 * @c lock é o mutex utilizado para controlar o acesso à variável @c tictactoe , realizando
 * a exclusão mútua de acesso à mesma. Fazendo com que não mais que uma thread possa adentrar
 * uma região de código trancada por essa variável.
 * 
 * @c tictactoe é um array que contém todos os jogos do servidor, e é utilizado pelas várias threads
 * de clientes para ler as informações da partida, entrar em partidas, etc.
 * 
 * @return resultado da inicialização: 0 em caso de sucesso e 1 em caso de erro.
 */
int init_shared_variables( void );

/**
 * @brief Thread de conexão dedicada a um cliente.
 *
 * Essa função é chamada em forma de thread sempre que um usuário realiza login no servidor, recebendo
 * o endereço @c client_address do client como parâmetro.
 * Essa função faz as seguintes ações:
 * - Realizar a abertura de um socket dedicado para o cliente, e envia o endereço do socket para o cliente.
 * - Receber e armazenar o nome do cliente conectado.
 * - Chamar a função @c search_for_available_game para encontrar uma partida para esse jogador.
 * - Chamar a função @c insert_player_in_game para inserir esse jogador na partida encontrada.
 * - Caso o jogador for o primeiro a se conectar a partida, espera até que o segundo se conecte
 *   chamando a função @c wait_all_players_to_connect.
 * - Caso o jogador for o segundo a se conectar na partida, realiza o sorteio de quem é o primeiro a jogar.
 * - Avisa os jogadores se são os primeiros ou segundos a jogar.
 * - Inicia a partida dos jogadores com a função @c start_remote_tictactoe_game.
 * - Fecha os socket abertos, ao fim da partida.
 * 
 * @param client_address Ponteiro para o endereço do cliente conectado.
 * @return Não há retorno.
 * 
 */
void *client_connection_thread(void *client_address);

/**
 * @brief Inicia uma partida de tic-tac-toe com o jogador indicado.
 *
 * Essa função implementa a troca de mensagens de um jogador com a partida, e deve ser chamada pelos dois jogadores.
 * Um é o jogador que foi sorteado para começar, o outro para ser o segundo a jogar.
 * A razão para isso é que a conexão dos dois clientes é tratada independentemente em duas threads, e a função é chamada
 * quase que simultaneamente pelas duas threads ao fim da função @c wait_all_players_to_connect.
 * 
 * Quando a partida acaba, é chamada a função @c leave_game.
 * 
 * @param socket Socket que o servidor esperará por mensagens desse jogador e enviará jogadas a seu oponente.
 * @param p Jogador que participará da partida.
 * @param game_id id da partida que está iniciando.
 * @return Não há retorno.
 * 
 */
void start_remote_tictactoe_game(int socket, Player p, unsigned short game_id);

/**
 * @brief Procura por um jogo com vagas abertas
 *
 * Faz a busca por uma partida com vagas abertas no servidor,
 * o procedimento de busca é o seguinte:
 * - Se houver uma partida com um player esperando um oponente, dá preferência a essa partida.
 * - Se não houver partida com um player esperando, procura pela primeira partida com duas vagas abertas.
 * - Se todas as partidas estiverem cheias, retorna @b -1 , indicando que não há vagas.
 * 
 * 
 * @return Id da partida encontrada.
 * 
 * <b>Exemplo de uso:</b>
 * @code
 * 
 * Player p;
 * int game_id;
 * 
 * game_id = search_for_available_game();
 * if (game_id == -1)
 * {
 *     printf("Não há partidas com vagas.\n");
 * }
 * else
 * {
 *     p.id = insert_player_in_game(p, game_id);
 * }
 * 
 * @endcode
 * 
 * @warning É recomendável que essa partida seja chamada dentro do bloqueio do mutex, para evitar que
 * as informações das partidas sejam alteradas durante a busca.
 */
int search_for_available_game( void );

/**
 * @brief Insere um jogador em uma partida.
 *
 * Insere o jogador @p p dentro da partida @p game_id, na primeira ou segunda posição.
 * 
 * @param p Jogador que será inserido.
 * @param game_id Partida que terá um jogador inserido.
 * @return @c int id do jogador dentro dessa partida, sendo 0 ou 1
 * 
 * <b>Exemplo de uso:</b>
 * @code
 * 
 * Player p;
 * int game_id;
 * 
 * game_id = search_for_available_game();
 * if (game_id == -1)
 * {
 *     printf("Não há partidas com vagas.\n");
 * }
 * else
 * {
 *     p.id = insert_player_in_game(p, game_id);
 * }
 * 
 * @endcode
 * 
 * @warning É recomendável que essa função seja chamada dentro do bloqueio do mutex. Para impedir
 * que mais de 2 jogadores tentem entrar na mesma partida ao mesmo tempo e quebre o limite de jogadores por partida.
 */
int insert_player_in_game(Player p, int game_id);

/**
 * @brief Espera até que todos os jogadores se conectem em uma partida. 
 *
 * @b Bloqueia a execução do programa até que a partida de id @c game_id esteja cheia.
 * 
 * @param game_id id da partida  
 * @return Não há retorno.
 * 
 * @warning Se essa função for chamada dentro do bloqueio do mutex, ela nunca será finalizada. 
 */
void wait_all_players_to_connect(int game_id);

/**
 * @brief Cria um arquivo com o nome dos vencedores. 
 *
 * Cria um novo arquivo ou abre um ja existente e escreve o nome do vencedor da partida.
 * 
 * @param msg nome do jogador vencedor  
 * @return Não há retorno.
 * 
 */
void write_in_file_ranking(char *msg);

/**
 * @brief Abandona uma partida
 * 
 * Decrementa em um o número de jogadores de uma partida;
 * 
 * @param game_id id da partida. 
 * @return Não há retorno.
 */
void leave_game(int game_id);