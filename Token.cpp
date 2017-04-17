#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Types.h"

using namespace std;

class Token {

private:
	Kind kind;
	int lineNumber;
	int colNumber;
	string* identifierPtr;
	string literal;

public:
	Token(Kind kind, int line, int col, string lit = "", string* ident = NULL)
	{
		this->kind = kind;
		this->lineNumber = line;
		this->colNumber = col;
		this->literal = lit;
		this->identifierPtr = ident;
	}

	~Token()
	{
		identifierPtr = NULL;
	}

	void setKind(Kind kind)
	{
		this->kind = kind;
	}

	void setLiteral(string lit)
	{
		this->literal = lit;
	}

	void printOut()
	{
		cerr << '[' << lineNumber << ',' << colNumber << ']' << ' ';
		switch (kind) //print out the kind. List matches list of Kind enums above
		{
		case UNARY_NEG: cerr << "UNARY_NEG"; break;
		case UNARY_NOT: cerr << "UNARY_NOT"; break;
		case UNARY_TILDE: cerr << "UNARY_TILDE"; break;
		case MULT: cerr << "MULT"; break;
		case DIV: cerr << "DIV"; break;
		case PLUS: cerr << "PLUS"; break;
		case MINUS: cerr << "MINUS"; break;
		case LESS_THAN: cerr << "LESS_THAN"; break;
		case GREAT_THAN: cerr << "GREAT_THAN"; break;
		case LESS_THAN_EQUAL: cerr << "LESS_THAN_EQUAL"; break;
		case GREATER_THAN_EQUAL: cerr << "GREATER_THAN_EQUAL"; break;
		case EQUALITY: cerr << "EQUALITY"; break;
		case INEQUALITY: cerr << "INEQUALITY"; break;
		case BITWISE_AND: cerr << "BITWISE_AND"; break;
		case LOGICAL_AND: cerr << "LOGICAL_AND"; break;
		case BITWISE_OR: cerr << "BITWISE_OR"; break;
		case LOGICAL_OR: cerr << "LOGICAL_OR"; break;
		case ASSIGNMENT: cerr << "ASSIGNMENT"; break;
		case HAT: cerr << "HAT"; break;
		case BITWISE_SHIFT_RIGHT: cerr << "BITWISE_SHIFT_RIGHT"; break;
		case BITWISE_SHIFT_LEFT: cerr << "BITWISE_SHIFT_LEFT"; break;
		case LEFT_PAREN: cerr << "LEFT_PAREN"; break;
		case RIGHT_PAREN: cerr << "RIGHT_PAREN"; break;
		case LEFT_BRACKET: cerr << "LEFT_BRACKET"; break;
		case RIGHT_BRACKET: cerr << "RIGHT_BRACKET"; break;
		case LEFT_BRACE: cerr << "LEFT_BRACE"; break;
		case RIGHT_BRACE: cerr << "RIGHT_BRACE"; break;
		case SEMICOLON: cerr << "SEMICOLON"; break;
		case BYTE: cerr << "BYTE"; break;
		case CONST: cerr << "CONST"; break;
		case ELSE: cerr << "ELSE"; break;
		case END: cerr << "END"; break;
		case EXIT: cerr << "EXIT"; break;
		case FLOAT64: cerr << "FLOAT64"; break;
		case FOR: cerr << "FOR"; break;
		case FUNCTION: cerr << "FUNCTION"; break;
		case IF: cerr << "IF"; break;
		case INT32: cerr << "INT32"; break;
		case PRINT: cerr << "PRINT"; break;
		case RECORD: cerr << "RECORD"; break;
		case REF: cerr << "REF"; break;
		case RETURN: cerr << "RETURN"; break;
		case STATIC: cerr << "STATIC"; break;
		case TYPE: cerr << "TYPE"; break;
		case VAR: cerr << "VAR"; break;
		case WHILE: cerr << "WHILE"; break;
		case INT_LITERAL: cerr << "INT_LITERAL"; break;
		case FLOAT_LITERAL: cerr << "FLOAT_LITERAL"; break;
		case BYTE_LITERAL: cerr << "BYTE_LITERAL"; break;
		case IDENTIFIER: cerr << "IDENTIFIER"; break;
		case COMMA: cerr << "COMMA"; break;
		case DOT: cerr << "DOT"; break;
		case STRING_LITERAL: cerr << "STRING_LITERAL"; break;
		case CHAR_LITERAL: cerr << "CHAR_LITERAL"; break;
		}
		if (literal.length() > 0)
			cerr << " (" << literal << ")";

		cerr << "\n";
	}
};