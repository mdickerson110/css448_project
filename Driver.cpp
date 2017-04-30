#include <iostream>
#include <fstream>
#include "Lexer.cpp"
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

	//lexer.lex();

	//cout << "Printing symbols..." << endl;
	//lexer.printSymbolTable();

	while (lexer.hasNext())
	{
		//DEBUG cout << "Beginning iteration..." << endl;

		Token *token = lexer.next();
		token->printOut();

		delete token;
		token = NULL;

		//DEBUG cout << "End of iteration..." << endl;
	}

	return 0;
}