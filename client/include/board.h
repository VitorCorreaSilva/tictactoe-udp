#include <stdio.h>
#include <stdlib.h>

//Limpa o console e desenha o tabuleiro
void DrawBoard(char gameBoard[9]);

//Versao alternativa da funcao anterior, para caso seja necessario informar de qual jogador a vez
void DrawBoardWithNames(char gameBoard[9], char *player1Name, char *player2Name, char *playerCharacter);

//Verifica se houve uma vitoria (1), empate (2) ou se o jogo continua (0)
int CheckPlayerWin(char gameBoard[9]);


int checkValidPlay(char *gameBoard, int position);
