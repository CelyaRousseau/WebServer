#include <iostream>           // std::cout
#include <stdio.h>

using namespace std;

int main( int argc, const char* argv[] )
{
	if(argc == 2) {
		std::string type = argv[1];
		cout << "Vous avez demandé le type de serveur : " << type.c_str() << endl;


	} else cout << "Indiquer le type de serveur désiré !" << endl;
}
