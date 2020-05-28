#include <game.h>

void PrintScore(int player1Wins, int player2Wins, int draws, char *player1Name, char *player2Name)
{
	printf("\nPontuacao: \n");
	printf("%s: %d vitorias\n", player1Name, player1Wins);
	printf("%s: %d vitorias\n", player2Name, player2Wins);
	printf("Empates: %d\n", draws);
}

//Escreve na tela o nome do vencedor, se houve algum, ou se houve empate
void PrintWinner(char *playerCharacter, char *player1Name, char *player2Name, int check)
{
	if(check == 1)
	{
		if(*playerCharacter == 'X')
		{
			printf("\n%s venceu!\n", player1Name);
		}
		else
		{
			printf("\n%s venceu!\n", player2Name);
		}
	}
	else if(check == 2)
	{
		printf("\nEmpate!\n");
	}
}

//Verifica se o usu�rio deseja iniciar uma nova partida, caso n�o queira, retorna para o menu
void PlayAgain(int *newGame)
{
	//Controle para que o usu�rio responda uma op��o v�lida
  	char answer;
  	do
  	{
  		printf("\nDeseja jogar mais uma partida? (s/n)");
  		if(scanf(" %c", &answer) ==  1)
		{
			answer = tolower(answer);
			if(answer == 'n')
			{
				*newGame = 1;	//Altera��o da vari�vel que define se haver� uma nova partida
			}
		}	
	}
	while(answer != 's' && answer != 'n');
}

//Define do nome dos jogadores e chama a funcao de inicio do jogo
void StartGame(int numberOfPlayers)
{  
  //Definicao das variaveis do(s) nome(s) do(s) jogador(es)
  char *player1Name, *player2Name;
  
  //Definicao do(s) nomes do(s) jogadore(s)
  if(numberOfPlayers == 1)
  {
	player1Name = GetPlayerName(0);	//Chama a funcao que define o nome do jogador (0 � o par�metro que define a mensagem para apenas um jogador)
    player2Name = "Computador";
  }
  else
  {
  	player1Name = GetPlayerName(1);	//Chama a funcao que define o nome do jogador (1 � o par�metro que define a mensagem para o jogador 1)
  	player2Name = GetPlayerName(2);	//Chama a funcao que define o nome do jogador (2 � o par�metro que define a mensagem para o jogador 2)
  }
  
  //Chamada da fun��o que inicia o jogo
  PlayGame(player1Name, player2Name, numberOfPlayers);
  
  //Libera a mem�ria alocada para os nomes dos jogadores
  free(player1Name);
  
  if(numberOfPlayers == 2)	//Caso sejam 2 jogadores, � necess�rio liberar a mem�ria do nome do jogador 2
  {
  	free(player2Name);
  }
}

//Define a jogada do computador
int AIPlay(char gameBoard[9], int *chosenNumber)
{
	//Garantia de que ser� escolhido um n�mero v�lido
	do
  	{
  		//Define um valor aleat�rio entre 1 e 9
  		*chosenNumber = (rand() % 9) + 1;
  	}
  	while(gameBoard[*chosenNumber - 1] == 'X' || gameBoard[*chosenNumber - 1] == 'O' || *chosenNumber > 9 || *chosenNumber <= 0);
  	
  	return *chosenNumber;	//Retorno do valor escolhido
}

//Define e mostra na tela qual jogador ir� come�ar a partida
void PlayerStart(char *playerCharacter, int *playerTurn, int numberOfPlayers, char *player1Name, char *player2Name)
{
	srand(time(0));	//Semente que define n�meros aleat�rios diferentes durante a execu��o
	
	//Valor aleat�rio que define quem inicia o a partida
  	int playerStart = (rand() % 2) + 1;
  	if(system("clear") == 0){}
  	
  	//Altera o caractere para o do jogador que inicia a partida caso necess�rio
  	if(playerStart == 2)
  	{
  		ChangePlayer(playerCharacter);
  		
  		//Caso seja um jogo contra o computador, define que ser� o turno do computador
		if(numberOfPlayers == 1)
		{
			*playerTurn = 1;
		}
		printf("\n%s comeca a partida!\n", player2Name);
		getchar();
	}
	else
	{
		printf("\n%s comeca a partida!\n", player1Name);
		getchar();
	}
}

//Atualiza os placares e altera o valor da vari�vel que controla o loop (win)
void UpdateScores(int check, char *playerCharacter, int *player1Wins, int *player2Wins, int *draws, char gameBoard[9])
{
	if(check == 1)	//Verifica se algu�m venceu
	{
		if(*playerCharacter == 'X')
		{
			*player1Wins += 1;
		}
		else
		{
			*player2Wins += 1;
		}
    	DrawBoard(gameBoard);
	}
	else if(check == 2)	//Verifica se houve empate
	{
		*draws += 1;
    	DrawBoard(gameBoard);
	}
}

//Fun��o que controla o fluxo do jogo
void PlayGame(char *player1Name, char *player2Name, int numberOfPlayers)
{
  int chosenNumber;	//Vari�vel que define qual a posi��o escolhida do tabuleiro

  //Vari�veis de controle de: nro de vit�rias do jogodor1, nro de vit�rias do jogador 2,
  //nro de empates, se o jogador decidir� come�ar um novo jogo (0 = false) e se � o turno do computador (0 = false)
  int player1Wins = 0, player2Wins = 0, draws = 0, newGame = 0, AITurn = 0;	
  
  //Loop que controla a quantidade de partidas, caso o jogador decida sair, o looping acaba
  while(newGame == 0)
  {
  	int win = 0;	//Controla se houve uma vit�ria (0 = false)
  	char character = 'X';	//Caractere do jogador atual
  	char *playerCharacter = &character;	//Ponteiro para a vari�vel do caractere do jogador
  	
  	//Chamada da fun��o de quem define o jogo
  	PlayerStart(playerCharacter, &AITurn, numberOfPlayers, player1Name, player2Name);
  	
  	//Defini��o do vetor inicial do jogo
  	char gameBoard[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
  	
  	//Loop da partida, caso haja uma vit�ria, o loop acaba
 	while(win == 0)
  	{
  		//Caso seja o turno do computador
  		if(AITurn == 1)
  		{
		  	AIPlay(gameBoard, &chosenNumber);	//Chamada da fun��o que retorna a posi��o definida pelo computador
  			gameBoard[chosenNumber - 1] = *playerCharacter;	//Altera��o da posi��o no tabuleiro
  			AITurn = 0;	//Define que n�o � mais o turno do computador
  			
  			//Verifica se o computador venceu ou empatou (0 = nada, 1 = vit�ria, 2 = empate)
	    	int check = CheckPlayerWin(gameBoard);
	    	
	    	//Caso tenha acabado a partida, atualiza os placares
		    if(check == 1 || check == 2)
		    {
		    	win = 1;	//Define que a partida acabou
		    	UpdateScores(check, playerCharacter, &player1Wins, &player2Wins, &draws, gameBoard);	//Atualiza os placares
		    	PrintWinner(playerCharacter, player1Name, player2Name, check);	//Informana tela se algu�m venceu ou empatou
		    	continue;
			}
		    ChangePlayer(playerCharacter);	//Muda o caractere para o do pr�ximo jogador
		}
  		//Desenha o tabuleiro
    	DrawBoardWithNames(gameBoard, player1Name, player2Name, playerCharacter);
    	
    	//Turno do jogador
    	//Garantia de que o jogador ir� escolher uma posi��o v�lida
  		do
  		{
    		//Recebe a posicao escolhida pelo jogador
    		if(scanf(" %d", &chosenNumber) == 1){}
  		}
  		while(gameBoard[chosenNumber - 1] == 'X' || gameBoard[chosenNumber - 1] == 'O' || chosenNumber > 9 || chosenNumber <= 0);
  		
		//Altera o valor no vetor do jogo
		gameBoard[chosenNumber - 1] = *playerCharacter;
	    DrawBoardWithNames(gameBoard, player1Name, player2Name, playerCharacter);
	    
	    //Verifica se algum jogador venceu ou empatou (0 = nada, 1 = vit�ria, 2 = empate)
	    int check = CheckPlayerWin(gameBoard);
	    
	    //Caso haja uma vit�ria ou empate, os placares s�o atualizados
	    if(check == 1 || check == 2)
		{
			win = 1; //Define que a partida acabou
		   	UpdateScores(check, playerCharacter, &player1Wins, &player2Wins, &draws, gameBoard);	//Atualiza os placares
			PrintWinner(playerCharacter, player1Name, player2Name, check);	//Informa na tela se houve vit�ria ou empate
		   	continue;
		}
	    
	    //Altera o caractere do jogador
	    ChangePlayer(playerCharacter);
	    
	    //Muda a vari�vel para o turno do computador, caso haja apenas 1 jogador
	    if(numberOfPlayers == 1)
	    {
	    	AITurn = 1;	
		}
	    
  	}	//Final do looping da partida
  	
  	//Escreve na tela o placar atual da partida
  	PrintScore(player1Wins, player2Wins, draws, player1Name, player2Name);
  	
  	//Define se uma nova partida ir� come�ar
  	PlayAgain(&newGame);
  	
  }	//Final do looping do jogo
}
