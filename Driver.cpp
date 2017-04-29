#include <iostream>
#include <fstream>
#include "parser.cpp"

using namespace std;

int main(int argc, char **argv)
{
	string inFile(argv[1]);	
	Parser parser;
	parser.parse(inFile);
}