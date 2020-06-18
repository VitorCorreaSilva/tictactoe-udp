/**
 * @file board.h
 * @author Vitor Correa da Silva
 * @date 29 May 2020
 * 
 * @brief Arquivo contendo as estruturas e cabeçalhos de funções do tabuleiro.
 *
 * Esse arquivo contém as definições das funções que são implementadas em board.c.
 * Essas funções tem como objetivo implementar o tabuleiro do jogo e suas especificações.
 * 
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Constroi o tabuleiro do jogo
 *
 * Limpa a tela e desenha o tabuleiro de jogo.
 * 
 * @param gameBoard Array de 9 posições que guarda as informações do tabuleiro
 * 
 */
void DrawBoard(char gameBoard[9]);

/**
 * @brief Constroi o tabuleiro do jogo
 *
 * Limpa a tela, desenha o tabuleiro e informa de qual jogador é a vez.
 * 
 * @param gameBoard Array de 9 posições que guarda as informações do tabuleiro
 * @param player1Name Nome do jogador numero 1
 * @param player2Name Nome do jogador numero 2
 * @param playerCharacter Caractere do jogador atual
 * 
 */
void DrawBoardWithNames(char gameBoard[9], char *player1Name, char *player2Name, char *playerCharacter);

/**
 * @brief Constroi o tabuleiro do jogo
 *
 * Verifica se houve uma vitoria (1), empate (2) ou se o jogo continua (0)
 * 
 * @param gameBoard Array de 9 posições que guarda as informações do tabuleiro
 * @return @c int Verifica se a jogada foi a vencedora
 * 
 */
int CheckPlayerWin(char gameBoard[9]);

/**
 * @brief Verifica a jogada
 *
 * Verifica se a jogada feita é valida.
 * 
 * @param gameBoard Array de 9 posições que guarda as informações do tabuleiro
 * @param position Valor da possivel jogada
 * @return @c int retorna a jogada feita se for valida
 * 
 */
int checkValidPlay(char *gameBoard, int position);

/**
 * @brief Limpa a tela
 *
 * Implementa função para lipar a tela.
 * 
 */
void clear_screen( void );