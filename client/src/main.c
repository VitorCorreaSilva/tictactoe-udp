#include <menu.h>
#include <player.h>
#include <game.h>
#include <client.h>

typedef enum opcao
{
    jogo_local = 1, jogo_rede, sair
}Opcao;

int main(){

    Opcao op;	//Variavel que define a escolha do usuario
	int numberOfPlayers;
    int socket;
	int login_server_port;
	int game_server_port;
    char game_server_ip[16];

    do
    {
        ShowMenu();
        if(scanf("%d", (int*)&op) != 1)
        {
            printf("Erro ao ler entrada do teclado. \n");
        }
        
        clear_stdin();

        switch(op)
        {
            case jogo_local:
                numberOfPlayers = GetNumberOfPlayers();
                StartGame(numberOfPlayers);
                break;
            case jogo_rede:
                printf("Digite o ip do server: ");
                if(scanf("%s", game_server_ip) != 1)
                {
                    printf("entrada invalida... \n");
                    sleep(1);
                    break;
                }
                printf("Digite a porta do server: ");
                if(scanf("%d", &login_server_port) != 1)
                {
                    printf("entrada invalida... \n");
                    sleep(1);
                    break;
                }
                printf("conectando...\n");
                clear_stdin();

                srand(time(0));
                socket = create_client_socket();

                game_server_port = login(socket, game_server_ip, login_server_port);
                if (game_server_port == 0)
                {
                    printf("Erro ao conectar ao servidor.\n");
                }
                else
                {
                    play_tictactoe(socket, game_server_ip, game_server_port);
                }                

                break;            
            case sair:
                printf("Encerrando...\n");
                break;
        }
    }while(op != sair);
    
    return 0;
}