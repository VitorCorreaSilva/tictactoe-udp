#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Muda o caractere do jogador atual
void ChangePlayer(char *playerCharacter);

//Retorna o numero de jogadores escolhido pelo usuario (1 ou 2)
int GetNumberOfPlayers();

//Define o nome do jogador (O par�metro de numero de jogadores define qual a mensagem mostrada no console e informa qual jogador deve informar o nome)
char* GetPlayerName(int playerNumber);	//Caso seja contra o computador, playerNumber 0, caso seja contra outro jogador, playerNumber 1 ou 2
