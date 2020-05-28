#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <player.h>
#include <board.h>

//Escreve a pontuacao atual dos jogadores na tela
void PrintScore(int player1Wins, int player2Wins, int draws, char *player1Name, char *player2Name);

//Escreve na tela o nome do vencedor, se houve algum, ou se houve empate
void PrintWinner(char *playerCharacter, char *player1Name, char *player2Name, int check);

//Verifica se o usuario deseja iniciar uma nova partida, caso nao queira, retorna para o menu
void PlayAgain(int *newGame);

//Define do nome dos jogadores e chama a funcao de inicio do jogo
void StartGame(int numberOfPlayers);

//Define a jogada do computador
int AIPlay(char gameBoard[9], int *chosenNumber);

//Define e mostra na tela qual jogador ira comecar a partida
void PlayerStart(char *playerCharacter, int *playerTurn, int numberOfPlayers, char *player1Name, char *player2Name);

//Atualiza os placares e altera o valor da variavel que controla o loop (win)
void UpdateScores(int check, char *playerCharacter, int *player1Wins, int *player2Wins, int *draws, char gameBoard[9]);

//Funcao que controla o fluxo do jogo
void PlayGame(char *player1Name, char *player2Name, int numberOfPlayers);
