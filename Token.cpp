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
	int columnNumber;
	string* symbolPtr;
	string literal;

public:

	Token()
	{
		this->kind = kind;
		this->lineNumber = 0;
		this->columnNumber = 0;
		this->literal = "";
		this->symbolPtr = NULL;
	}

	Token(Kind kind, int line, int col, string literal = "", string* symbol = NULL)
	{
		this->kind = kind;
		this->lineNumber = line;
		this->columnNumber = col;
		this->literal = literal;
		this->symbolPtr = symbol;
	}

	~Token()
	{
		//only set symbolPtr to null, as there is still a reference
		//to the string pointed to by symbolPtr in the Lexer's symbol table.
		symbolPtr = NULL;
	}

	void setKind(Kind kind)
	{
		this->kind = kind;
	}

	Kind getKind()
	{
		return this->kind;
	}

	void setLiteral(string literal)
	{
		this->literal = literal;
	}

	void setSymbolPtr(string* symbol)
	{
		this->symbolPtr = symbol;
	}

	void printOut()
	{
		cout << '[' << lineNumber << ',' << columnNumber << ']' << ' ';
		switch (kind) //print out the kind. List matches list of Kind enums above
		{
			case ASSIGNMENT: cout << "ASSIGNMENT"; break;
			case BITWISE_AND: cout << "BITWISE_AND"; break;
			case BITWISE_OR: cout << "BITWISE_OR"; break;
			case BITWISE_SHIFT_LEFT: cout << "BITWISE_SHIFT_LEFT"; break;
			case BITWISE_SHIFT_RIGHT: cout << "BITWISE_SHIFT_RIGHT"; break;
			case BYTE: cout << "BYTE"; break;
			case BYTE_LITERAL: cout << "BYTE_LITERAL"; break;
			case CHAR_LITERAL: cout << "CHAR_LITERAL"; break;
			case COMMA: cout << "COMMA"; break;
			case CONST: cout << "CONST"; break;
			case DIV: cout << "DIV"; break;
			case DOT: cout << "DOT"; break;
			case ELSE: cout << "ELSE"; break;
			case END: cout << "END"; break;
			case EQUALITY: cout << "EQUALITY"; break;
			case EXIT: cout << "EXIT"; break;
			case FLOAT_LITERAL: cout << "FLOAT_LITERAL"; break;
			case FLOAT64: cout << "FLOAT64"; break;
			case FOR: cout << "FOR"; break;
			case FUNCTION: cout << "FUNCTION"; break;
			case GREAT_THAN: cout << "GREAT_THAN"; break;
			case GREATER_THAN_EQUAL: cout << "GREATER_THAN_EQUAL"; break;
			case HAT: cout << "HAT"; break;
			case IDENTIFIER: cout << "IDENTIFIER"; break;
			case IF: cout << "IF"; break;
			case INEQUALITY: cout << "INEQUALITY"; break;
			case INT_LITERAL: cout << "INT_LITERAL"; break;
			case INT32: cout << "INT32"; break;
			case INVALID: cout << "INVALID"; break;
			case LEFT_BRACE: cout << "LEFT_BRACE"; break;
			case LEFT_BRACKET: cout << "LEFT_BRACKET"; break;
			case LEFT_PAREN: cout << "LEFT_PAREN"; break;
			case LESS_THAN: cout << "LESS_THAN"; break;
			case LESS_THAN_EQUAL: cout << "LESS_THAN_EQUAL"; break;
			case LOGICAL_AND: cout << "LOGICAL_AND"; break;
			case LOGICAL_OR: cout << "LOGICAL_OR"; break;
			case MINUS: cout << "MINUS"; break;
			case MULT: cout << "MULT"; break;
			case PLUS: cout << "PLUS"; break;
			case PRINT: cout << "PRINT"; break;
			case RECORD: cout << "RECORD"; break;
			case REF: cout << "REF"; break;
			case RETURN: cout << "RETURN"; break;
			case RIGHT_BRACE: cout << "RIGHT_BRACE"; break;
			case RIGHT_BRACKET: cout << "RIGHT_BRACKET"; break;
			case RIGHT_PAREN: cout << "RIGHT_PAREN"; break;
			case SEMICOLON: cout << "SEMICOLON"; break;
			case STATIC: cout << "STATIC"; break;
			case STRING_LITERAL: cout << "STRING_LITERAL"; break;
			case TYPE: cout << "TYPE"; break;
			case UNARY_NEG: cout << "UNARY_NEG"; break;
			case UNARY_NOT: cout << "UNARY_NOT"; break;
			case UNARY_TILDE: cout << "UNARY_TILDE"; break;
			case VAR: cout << "VAR"; break;
			case WHILE: cout << "WHILE"; break;
		}

		if (literal.length() > 0)
		{
			cout << " (" << literal << ")";
		}

		cout << "\n";
	}
};