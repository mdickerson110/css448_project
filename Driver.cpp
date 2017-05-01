#include <iostream>
#include <fstream>
#include "lexer.cpp"
#include "Types.h"

using namespace std;

int main(int argc, char **argv)
{
	string inFile(argv[1]);
	Lexer lexer;
	
	if (lexer.openSource(inFile) == ERROR)
	{
		return -1;
	}

	while (lexer.hasNext())
	{
		//DEBUG cout << "Beginning iteration..." << endl;

		Token *token = lexer.next();
		token->printOut();

		delete token;
		token = NULL;

		//DEBUG cout << "End of iteration..." << endl;
	}

	lexer.printSymbolTable();

	return 0;
}