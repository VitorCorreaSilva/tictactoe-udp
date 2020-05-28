#include <board.h>

//Limpa o console e desenha o tabuleiro
void DrawBoard(char gameBoard[9])
{
  if(system("clear") == 0)
  {
	printf(" %c | %c | %c ", gameBoard[0], gameBoard[1], gameBoard[2]);
	printf("\n----------\n");
	printf(" %c | %c | %c ", gameBoard[3], gameBoard[4], gameBoard[5]);
	printf("\n----------\n");
	printf(" %c | %c | %c ", gameBoard[6], gameBoard[7], gameBoard[8]);
	printf("\n");
  }
}

//Versão alternativa da função anterior, para caso seja necessário informar de qual jogador é a vez
void DrawBoardWithNames(char gameBoard[9], char *player1Name, char *player2Name, char *playerCharacter)
{
  if(system("clear") == 0)
  {
	//Verifica quem está jogando e mostra o nome correspondente
	if(*playerCharacter == 'X')
	{
		printf("%s, sua vez\n\n", player1Name);
	}
	else
	{
		printf("%s, sua vez\n\n", player2Name);
	}
	printf(" %c | %c | %c ", gameBoard[0], gameBoard[1], gameBoard[2]);
	printf("\n----------\n");
	printf(" %c | %c | %c ", gameBoard[3], gameBoard[4], gameBoard[5]);
	printf("\n----------\n");
	printf(" %c | %c | %c ", gameBoard[6], gameBoard[7], gameBoard[8]);
  }
}

/**/
int checkValidPlay(char *gameBoard, int position)
{
	return !(gameBoard[position - 1] == 'X' || gameBoard[position - 1] == 'O' || position > 9 || position <= 0);
}

//Verifica se houve uma vitoria (1), empate (2) ou se o jogo continua (0)
int CheckPlayerWin(char gameBoard[9])
{
	//Verificação de todos os possíveis casos de vitória
	if((gameBoard[0] == gameBoard[1] && gameBoard[1] == gameBoard[2])
	|| (gameBoard[3] == gameBoard[4] && gameBoard[4] == gameBoard[5])
	|| (gameBoard[6] == gameBoard[7] && gameBoard[7] == gameBoard[8])
	|| (gameBoard[0] == gameBoard[3] && gameBoard[3] == gameBoard[6])
	|| (gameBoard[1] == gameBoard[4] && gameBoard[4] == gameBoard[7])
	|| (gameBoard[2] == gameBoard[5] && gameBoard[5] == gameBoard[8])
	|| (gameBoard[0] == gameBoard[4] && gameBoard[4] == gameBoard[8])
	|| (gameBoard[2] == gameBoard[4] && gameBoard[4] == gameBoard[6]))
	{
		return 1;
	}
	
	
	int i, counter = 0;
	//Verificação se todas as casas do tabuleiro estão ocupadas
	for(i = 0; i < 9; i++)
	{
		if(gameBoard[i] != 'X' && gameBoard[i] != 'O')
		{
			counter++;
		}
	}
	
	//Caso todas as casas estejam ocupadas, retorna 2, representando empate
	if(counter == 0)
	{
		return 2;
	}
	
	//Caso não seja empate ou vitória, retorna 0 e o jogo continua
	return 0;
}