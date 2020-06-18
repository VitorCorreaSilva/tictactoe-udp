/**
 * @file game.h
 * @author Vitor Correa da Silva
 * @date 29 May 2020
 * 
 * @brief Arquivo contendo as estruturas e cabeçalhos de funções do jogo.
 *
 * Esse arquivo contém as definições das funções que são implementadas em game.c.
 * Essas funções tem como objetivo implementar o jogo tic-tac-toe (jogo da velha).
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <player.h>
#include <board.h>

/**
 * @brief Mostrar a pontuação
 *
 * Escreve a pontuacao atual dos jogadores na tela.
 * 
 * @param player1Wins Numero de vitorias do jogador 1
 * @param player2Wins Numero de vitorias do jogador 2
 * @param draws Numero de empates entre os dois jogadores
 * @param player1Name Nome do jogador numero 1
 * @param player2Name Nome do jogador numero 2
 * 
 */
void PrintScore(int player1Wins, int player2Wins, int draws, char *player1Name, char *player2Name);

/**
 * @brief Mostrar a nome do vencedor
 *
 * Escreve na tela o nome do vencedor, se houve algum, ou se houve empate
 * 
 * @param playerCharacter Caractere do joogador. Pode ser @b X ou @b O
 * @param player1Name Nome do jogador 1
 * @param player2Name Nome do jogador 2
 * @param check Verificador de vitoria ou empate. @b 1 se houver vencedo ou @b 2 se for um empate
 * 
 */

void PrintWinner(char *playerCharacter, char *player1Name, char *player2Name, int check);

/**
 * @brief Verificar nova partida
 *
 * Verifica se o usuario deseja iniciar uma nova partida, caso nao queira, retorna para o menu.
 * 
 * @param newGame Verficador de novo jogo. @b 1 para começar uma nova partida
 * 
 */
void PlayAgain(int *newGame);

/**
 * @brief Começar uma partida
 *
 * Define do nome dos jogadores e chama a funcao de inicio do jogo.
 * 
 * @param numberOfPlayers Numero de jogadores na partida
 * 
 */
void StartGame(int numberOfPlayers);

/**
 * @brief Jogada do computador
 *
 * Implementa e simula a jogada do computador. Só é utilizada esta função quando 
 * apenas um jogador esta na partida.
 * 
 * @param gameBoard Array de 9 posições que guarda as informações do tabuleiro
 * @param chosenNumber Valor da possivel jogada
 * @return @c int Valor escolhido pelo jogador
 * 
 */
int AIPlay(char gameBoard[9], int *chosenNumber);

/**
 * @brief Definir primeiro jogador
 *
 * Define e mostra na tela qual jogador ira comecar a partida.
 * 
 * @param playerCharacter Caractere do jogador atual
 * @param playerTurn Define o turno do jogador
 * @param numberOfPlayers Numero de jogadores na partida
 * @param player1Name Nome do jogador numero 1
 * @param player2Name Nome do jogador numero 2
 * 
 */
void PlayerStart(char *playerCharacter, int *playerTurn, int numberOfPlayers, char *player1Name, char *player2Name);

/**
 * @brief Atualiza o marcador de pontos
 *
 * Atualiza os placares e altera o valor da variavel que controla o loop (win).
 *
 * @param check Verificador de vitoria ou empate. @b 1 se houver vencedo ou @b 2 se for um empate
 * @param playerCharacter Caractere do joogador. Pode ser @b X ou @b O
 * @param player1Wins Numero de vitorias do jogador 1
 * @param player2Wins Numero de vitorias do jogador 2
 * @param draws Numero de empates entre os dois jogadores
 * @param gameBoard Array de 9 posições que guarda as informações do tabuleiro
 * 
 */
void UpdateScores(int check, char *playerCharacter, int *player1Wins, int *player2Wins, int *draws, char gameBoard[9]);

/**
 * @brief Implementa o jogo e suas funções
 *
 * Funcao que controla o fluxo do jogo.
 *
 * @param player1Name Nome do jogador numero 1
 * @param player2Name Nome do jogador numero 2
 * @param numberOfPlayers Numero de jogadores na partida 
 * 
 */
void PlayGame(char *player1Name, char *player2Name, int numberOfPlayers);
