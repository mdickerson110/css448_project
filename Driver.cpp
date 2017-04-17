#include <iostream>
#include <fstream>
#include "Lexer.cpp"

using namespace std;

int main(int argc, char **argv)
{
	string inFile(argv[1]);
	Lexer lex;
	
	if (lex.openSource(inFile) == ERROR)
	{
		return -1;
	}

	lex.lex();
}