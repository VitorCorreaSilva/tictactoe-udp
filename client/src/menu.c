#include <menu.h>
#include <client.h>

//Mostra o menu
void ShowMenu()
{
	if(system("clear")==0)
	{
		printf("Menu");
		printf("\n1 - Jogar");
		printf("\n2 - Jogar em lan");
		printf("\n3 - Sair\n");
	}
}