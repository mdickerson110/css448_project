#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <map>
#include "IdentifierBuffer.cpp"

using namespace std;

//King of Symbeol. List is not complete yet
enum Kind {
	UNKNOWN, UNARY_NEG, UNARY_NOT, UNARY_TILDE, MULT, DIV, PLUS, MINUS, LESS_THAN, GREAT_THAN, LESS_THAN_EQUAL,
	GREATER_THAN_EQUAL, EQUALITY, INEQUALITY, BITWISE_AND, LOGICAL_AND, BITWISE_OR, LOGICAL_OR,
	ASSIGNMENT, HAT, BITWISE_SHIFT_LEFT, BITWISE_SHIFT_RIGHT, LEFT_PAREN, RIGHT_PAREN, LEFT_BRACKET, RIGHT_BRACKET, 
	LEFT_BRACE, RIGHT_BRACE, SEMICOLON, BYTE, CONST, ELSE, END, EXIT, FLOAT64, FOR,
	FUNCTION, IF, INT32, PRINT, RECORD, REF, RETURN, STATIC, TYPE, VAR, WHILE, INT_LITERAL, FLOAT_LITERAL,
	BYTE_LITERAL, IDENTIFIER, COMMA, DOT, STRING_LITERAL, CHAR_LITERAL
};



//*****************************************************************
		//TOKEN CLASS
//*****************************************************************
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
		if( literal.length() > 0)
			cerr << " (" << literal << ")";
		
		cerr << "\n";
	}			
};

//*****************************************************************
//HELPER FUNCTIONS
//*****************************************************************
//Check if input char is a digit
bool isNumber(char curChar)
{
	if(curChar == '0' || curChar == '1' || curChar == '2' || curChar == '3' || curChar == '4' || 
		curChar == '5' || curChar == '6' || curChar == '7' || curChar == '8' || curChar == '9')
		return true;
	else
		return false;
}

//Check if input char is a valid letter or '_'
bool isLetter(char curChar)
{
	if(curChar == 'A' || curChar == 'B' || curChar == 'C' || curChar == 'D' || curChar == 'E' || 
		curChar == 'F' || curChar == 'G' || curChar == 'H' || curChar == 'I' || curChar == 'J' ||
		curChar == 'K' || curChar == 'L' || curChar == 'M' || curChar == 'N' || curChar == 'O' || 
		curChar == 'P' || curChar == 'Q' || curChar == 'R' || curChar == 'S' || curChar == 'T' ||
		curChar == 'U' || curChar == 'V' || curChar == 'W' || curChar == 'X' || curChar == 'Y' || 
		curChar == 'Z' || curChar == 'a' || curChar == 'b' || curChar == 'c' || curChar == 'd' ||
		curChar == 'e' || curChar == 'f' || curChar == 'g' || curChar == 'h' || curChar == 'i' || 
		curChar == 'j' || curChar == 'k' || curChar == 'l' || curChar == 'm' || curChar == 'n' ||
		curChar == 'o' || curChar == 'p' || curChar == 'q' || curChar == 'r' || curChar == 's' || 
		curChar == 't' || curChar == 'u' || curChar == 'v' || curChar == 'w' || curChar == 'x' ||
		curChar == 'y' || curChar == 'z' || curChar == '_' )
		return true;
	else
		return false;
}

//check if input string is a keyword. If so, set the token Kind to this keyword.
bool isKeyword(string word, Token &token)
{
	if(word == "byte") token.setKind(BYTE);			
	else if(word == "const") token.setKind(CONST);	
	else if(word == "else") token.setKind(ELSE);	 
	else if(word == "end") token.setKind(END);	 
	else if(word == "exit") token.setKind(EXIT);	 
	else if(word == "float64") token.setKind(FLOAT64);	 
	else if(word == "for") token.setKind(FOR);	 
	else if(word == "function") token.setKind(FUNCTION);	 
	else if(word == "if") token.setKind(IF);	 
	else if(word == "int32") token.setKind(INT32);	 
	else if(word == "print") token.setKind(PRINT);	 
	else if(word == "record") token.setKind(RECORD);	 
	else if(word == "ref") token.setKind(REF);	 
	else if(word == "return") token.setKind(RETURN);	 
	else if(word == "static") token.setKind(STATIC);	 
	else if(word == "type") token.setKind(TYPE);	 
	else if(word == "var") token.setKind(VAR);	 
	else if(word == "while") token.setKind(WHILE);
	else return false;
	return true;
}

//Get the next char of the input and nextChar. Increment curChar and columnNumber
void getNextChar(ifstream &source, char &curChar, char &nextChar, int &columnNumber)
{
	curChar = nextChar;
	
	if(!source.get(nextChar))
	{
		nextChar = '\0';
	}
	
	columnNumber++;
}

//*****************************************************************
//MAIN LEXER FUNCTION
//*****************************************************************
//Take in file (code) and output a file with the list of tokens (output.txt)
int main(int argc,  char **argv)
{
	//Redirect stderr to output.txt
	FILE *stream;

	if((stream = freopen("output.txt", "w", stderr)) == NULL)
	{
		return -1;
	}

	//Open file input by user
	ifstream source;
	source.open(argv[1]);

	if (!source.is_open())
	{
		cerr << "Source File failed to open" << endl;
		source.close();
		return -1;
	}

	char curChar;
	char nextChar;
	
	//Get first char. If null, exit.
	if(!source.get(curChar))
	{
		cerr << "File is empty";
		source.close();
		return -1;
	}

	//Globals: Keeps track of location within file
	int lineNumber = 1;
	int columnNumber = 1;
	
	//Global: Temp buffer for storing strings.
	IdentifierBuffer idBuf;
	

	
	// loop getting single characters as long as the file lasts.
	while (curChar != '\0')
	{
		//if end of file, set nextChar to null
		if (!source.get(nextChar))
		{
			nextChar = '\0';	
		}
		
		//*****************************************************************
		//MAIN SWITCH
		//*****************************************************************
		//Switch statement for every possible char in the language
		switch(curChar)
		{	
			//*****************************************************************
			//OPERATORS
			//*****************************************************************
			case '-':
			{
				Token token(MINUS, lineNumber, columnNumber);
				token.printOut();
				break;
			}
			case '!':
			{
				Token token(UNKNOWN, lineNumber, columnNumber);
				
				if (nextChar == '=')
				{	// "!="
					token.setKind(INEQUALITY);
					getNextChar(source, curChar, nextChar, columnNumber);
				}
				else
				{	// "!"
					token.setKind(UNARY_NOT);
				}
				
				token.printOut();
				break;
			}
			case '&':
			{
				Token token(UNKNOWN, lineNumber, columnNumber);
				
				if (nextChar == '&')
				{
					token.setKind(LOGICAL_AND);
					getNextChar(source, curChar, nextChar, columnNumber);
				}
				else
				{
					token.setKind(BITWISE_AND);
				}
				
				token.printOut();
				break;
			}
			case '*':
			{
				Token token(MULT, lineNumber, columnNumber);
				token.printOut();
				break;
			}
			case '/':
			{
				//The DIV operator can also signify the beginning of a comment,
				//therefore this case needs to include logic to determine whether
				//curChar marks the beginning of a comment.
				
				if (nextChar == '/')
				{ 	//read comment until the end of line char or end of file
					while(curChar != '\n' && curChar != '\0')
					{	
						getNextChar(source, curChar, nextChar, columnNumber);
					}
					
					//update globals
					lineNumber++;
					columnNumber = 0;
					break;
				}
				else if (nextChar == '*')
				{ 	//read comment until '*/' is reached
			
					//Increment char pointers BEFORE while loop. If this operation
					//is not performed, then the string "/*/" may prematurely
					//exit the loop and fail to consume the full comment.
					getNextChar(source, curChar, nextChar, columnNumber);
					
					while (true)
					{
						getNextChar(source, curChar, nextChar, columnNumber);
						
						//check if the end of comment has been reached.
						if (curChar == '*' && nextChar == '/')
						{	//consume nextChar so that curChar will be set to
							//the character directly after nextChar at the
							//beginning of the next iteration of the main loop.
							getNextChar(source, curChar, nextChar, columnNumber);
							break;
						}
						//check for new line in multi-line comment
						else if (curChar == '\n')
						{	//Update global.
							lineNumber++;
						}
						//check for end of file
						else if (nextChar == '\0')
						{	//simply break out of loop. The end of file condition
							//will be caught at the beginning of the next
							//iteration of the main loop.
							break;
						}
					}
					
					//prevent falling through to the next case.
					break;
				}
				else
				{	//curChar is simply a DIV token.
					Token token(DIV, lineNumber, columnNumber);
					token.printOut();
					break;
				}
			}
			case '^':
			{
				Token token(HAT, lineNumber, columnNumber);
				token.printOut();
				break;
			}
			case '|':
			{
				Token token(UNKNOWN, lineNumber, columnNumber);
				
				if (nextChar == '|')
				{	// "||"
					token.setKind(LOGICAL_OR);
					getNextChar(source, curChar, nextChar, columnNumber);
				}
				else
				{	// "|"
					token.setKind(BITWISE_OR);
				}
				
				token.printOut();
				break;
			}
			case '~':
			{
				Token token(UNARY_TILDE, lineNumber, columnNumber);
				token.printOut();
				break;
			}
			case '+':
			{
				Token token(PLUS, lineNumber, columnNumber);
				token.printOut();
				break;
			}
			case '<':
			{
				Token token(UNKNOWN, lineNumber, columnNumber);
				
				if(nextChar == '<')
				{
					token.setKind(BITWISE_SHIFT_LEFT);
					getNextChar(source, curChar, nextChar, columnNumber);
				}
				else if(nextChar == '=')
				{
					token.setKind(LESS_THAN_EQUAL);
					getNextChar(source, curChar, nextChar, columnNumber);
				}
				else
				{
					token.setKind(LESS_THAN);
				}
				
				token.printOut();
				break;
			}
			case '>':
			{
				Token token(UNKNOWN, lineNumber, columnNumber);
				
				if(nextChar == '>')
				{
					token.setKind(BITWISE_SHIFT_RIGHT);
					getNextChar(source, curChar, nextChar, columnNumber);
				}
				else if(nextChar == '=')
				{
					token.setKind(GREATER_THAN_EQUAL);
					getNextChar(source, curChar, nextChar, columnNumber);
				}
				else
				{
					token.setKind(GREAT_THAN);
				}
				
				token.printOut();
				break;
			}
			case '=':
			{
				Token token(UNKNOWN, lineNumber, columnNumber);
				
				if (nextChar == '=')
				{
					token.setKind(EQUALITY);
					getNextChar(source, curChar, nextChar, columnNumber);
				}
				else
				{
					token.setKind(ASSIGNMENT);
				}
				
				token.printOut();
				break;
			}
			
			
			//*****************************************************************
			//SINGLE CHARACTER TOKENS
			//*****************************************************************
			//new line
			case '\n':
			{
				//update globals
				lineNumber++;
				columnNumber = 0;
				break;				
			}
			case '(':
			{
				Token token(LEFT_PAREN, lineNumber, columnNumber);
				token.printOut();
				break;
			}
			case ')':
			{
				Token token(RIGHT_PAREN, lineNumber, columnNumber);
				token.printOut();
				break;
			}
			case ';':
			{
				Token token(SEMICOLON, lineNumber, columnNumber);
				token.printOut();
				break;
			}
			case '[':
			{
				Token token(LEFT_BRACKET, lineNumber, columnNumber);
				token.printOut();
				break;
			}
			case ']':
			{
				Token token(RIGHT_BRACKET, lineNumber, columnNumber);
				token.printOut();
				break;
			}
			case '{':
			{
				Token token(LEFT_BRACE, lineNumber, columnNumber);
				token.printOut();
				break;
			}
			case '}':
			{
				Token token(RIGHT_BRACE, lineNumber, columnNumber);
				token.printOut();
				break;
			}			
			case ',':
			{
				Token token(COMMA, lineNumber, columnNumber);
				token.printOut();
				break;
			}
			case '\t':
			{	//simply consume tab characters
				break;
			}
			case ' ':
			{	//simply consume white space
				break;
			}
			
			//*****************************************************************
			//STRING LITERAL
			//*****************************************************************
			case '"':
			{
				//simply read the string until the matching double quote.
				//The lexer does not need to concern itself with interpreting
				//escape characters; it needs only to extract a string.
				Token token(STRING_LITERAL, lineNumber, columnNumber);
				
				//consume '"'
				getNextChar(source, curChar, nextChar, columnNumber);
				
				while (curChar != '"')
				{
					idBuf.addChar(curChar);
					getNextChar(source, curChar, nextChar, columnNumber);
				}
				
				//TODO: Store string inside a data structure.
				token.setLiteral(idBuf.getIdentifier());
				token.printOut();
				break;
			}

			//*****************************************************************
			//CHARACTER LITERAL
			//*****************************************************************
			case '\'':
			{
				//Simply read the character literal until the matching single quote.
				Token token(CHAR_LITERAL, lineNumber, columnNumber);
				
				//consume '\'' (single quote)
				getNextChar(source, curChar, nextChar, columnNumber);
				
				//NOTE: Character literals are represented by multiple characters
				//in source code.
				while (curChar != '\'')
				{
					idBuf.addChar(curChar);
					getNextChar(source, curChar, nextChar, columnNumber);
				}
				
				token.setLiteral(idBuf.getIdentifier());
				token.printOut();
				break;
			}
			
			//*****************************************************************
			//INTEGER OR FLOATING POINT NUMBER
			//*****************************************************************
 			case '0': case '1': case '2': case '3': case '4': 
			case '5': case '6': case '7': case '8': case '9': case '.': 
			{
				if(curChar == '.')
				{
					if(isNumber(nextChar) == false)
					{
						Token token(DOT, lineNumber, columnNumber);
						token.printOut();
						break;
					}
				}
				
				Token token(INT_LITERAL, lineNumber, columnNumber);			
				IdentifierBuffer numBuffer;				
				bool hasDot = false;
				bool hasError = false;
				
				//Loop to get complete number
				while(true)
				{
					numBuffer.addChar(curChar);
					
					if(curChar == '.')
					{
						if (hasDot == true)
						{
							cerr << "Syntax Error. Dot in wrong place" << endl;
							hasError = true;
							
							//NOTE: Not sure if this is valid. Check specification
							// for numbers.
							
							//Consume the rest of the invalid token
							while(nextChar != ' ' && nextChar != '\n' && nextChar != '\t')
							{
								getNextChar(source, curChar, nextChar, columnNumber);
//								if(!source.get(nextChar))
//									break;
							}	
							break;	
						}
						
						token.setKind(FLOAT_LITERAL);
						hasDot = true;
					}
					
					//If there is more to the number
					if(isNumber(nextChar) || nextChar == '.')
					{
						getNextChar(source, curChar, nextChar, columnNumber);
					}
					else //Number is completed
					{
						break;
					}
				}
				
				//Update and print token if it is valid
				if(!hasError)
				{
					token.setLiteral(numBuffer.getIdentifier());
					token.printOut();
				}
				
				break;
			}
			 
			//******************************************************************
			//The following cases check for reserved words. If a reserved word
			//	is not detected, then execution will fall through to the default
			//	case. If the default case is reached, then the current token
			//	is an identifier. The default case reads all of the remaining tokens
			//	of the identifier.
			//******************************************************************

			//check for "byte"
/* 			case 'b':
				if(nextChar == 'y'))
				{
					curChar = nextChar;
					source.get(nextChar);
				}

			case 'c':
				//check for "const"
				break;
			case 'e':
				//check for "else", "end", or "exit"
				break;
			case 'f':
				//check for "float64", "for", or "function"
				break;
			case 'i':
				//check for "if" or "int32"
				break;
			case 'p':
				//check for "print"
				break;
			case 'r':
				//check for "record", "ref", or "return"
				break;
			case 's':
				//check for "static"
				break;
			case 't':
				//check for "type"
				break;
			case 'v':
				//check for "var"
				break;
			case 'w':
				//check for "while"
				break; */
				
			
			//default:
			//case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H':
			//case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P':
			//case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
			//case 'Y': case 'Z': case 'a': /* 'b' */ /* 'c' */ case 'd': /* 'e' */ /* 'f' */
			//case 'g': case 'h': /* 'i' */ case 'j': case 'k': case 'l': case 'm': case 'n':
			//case 'o': /* 'p' */ case 'q': /* 'r' */ /* 's' */ /* 't' */ case 'u': /* 'v' */
			///* 'w' */ case 'x': case 'y': case 'z': case '_':
			//	break;
			
			//*****************************************************************
			//Identifiers
			//*****************************************************************
			case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': 
			case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': 
			case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': 
			case 'Y': case 'Z': case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': 
			case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': 
			case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': 
			case 'w': case 'x': case 'y': case 'z': case '_':
			{
				Token token(IDENTIFIER, lineNumber, columnNumber);			
				IdentifierBuffer idBuffer;				
				
				//Loop to get complete identifier
				while(true)
				{
					if(idBuffer.addChar(curChar) == false)
						cerr << "Error: Identifier has more than 64 characters" << endl;
					
					//If there is more to the identifier
					if(isLetter(nextChar) || isNumber(nextChar))
						getNextChar(source, curChar, nextChar, columnNumber);
					else //Number is completed					
						break;
				}
				
				string ident = idBuffer.getIdentifier();
				if(isKeyword(ident, token) == false)
					token.setLiteral(ident);
				token.printOut();
				break;
			}
			
			//If char matches none of the cases, it is invalid
			default: 
			cerr << "invalid token" << endl;
		}

		curChar = nextChar;
		columnNumber++;
	}

	//close file and exit program
	source.close();
}
