/**
 * @file menu.h
 * @author Vitor Correa da Silva
 * @date 29 May 2020
 * 
 * @brief Arquivo contendo as estruturas e cabeçalhos de funções do menu.
 *
 * Esse arquivo contém as definições das funções que são implementadas em menu.c.
 * Essas funções tem como objetivo implementar um menu interativo.
 * 
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Mostra o menu
 * 
 * Implemtação do texto que é mostrado no menu
 * 
 */
void ShowMenu();

/**
 * @brief Chama a função escolhida pelo jogador
 *
 * Espera um @b int para definir a escolha do jogador e depois faz a chamada da função escolhida.
 * 
 * @param choice Escolha do jogador
 * @param numberOfPlayers Numero de jogadores
 * 
 */
//Chama a funcao escolhida pelo jogador
void MenuChoice(int choice, int *numberOfPlayers);
