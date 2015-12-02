#include <iostream>           // std::cout
#include <stdio.h>


int main( int argc, const char* argv[] )
{
	if(argc == 1) {
		string type = argv[0];
		printf("Vous avez demandé le type de serveur : %s", type);


	} else printf("Indiquer le type de serveur désiré !");
}