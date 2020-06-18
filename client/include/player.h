/**
 * @file player.h
 * @author Vitor Correa da Silva
 * @date 29 May 2020
 * 
 * @brief Arquivo contendo cabeçalhos de funções que implementam as funções do player.
 *
 * Esse arquivo contém as definições das funções que são implementadas em player.c.
 * Essas funções tem como objetivo implementar as operações de escolha de caracter para os jogadores, escolha do numero 
 * de jogadores e define o nome do jogador.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Muda o caractere do jogador atual
 *
 * Decide o caractere a ser usado pelo jogador. 
 * 
 * @param playerCharacter Ponteiro que guarda a informação de caractere do jogador.
 * 
 */
void ChangePlayer(char *playerCharacter);

/**
 * @brief Retorna o numero de jogadores
 *
 * Implementa a escolha do numero de jogadores 1 ou 2.
 * 
 * @return @c int contendo o numero de jogadores da partida. Retorna @b 1 ou @b 2 jogadores.
 * 
 */
int GetNumberOfPlayers();

/**
 * @brief Define o nome do jogador
 *
 * Define o nome do jogador que é retornornado como ponteiro @c *char
 * 
 * @param playerNumber Contem o numero do jogador que deve alterar o nome. @b 0 para computador @b 1 ou @b 2 para os jogadores.
 * @return @c *char contendo o nome do jogador.
 * 
 */
char* GetPlayerName(int playerNumber);
