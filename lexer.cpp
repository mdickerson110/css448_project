#include <iostream>
#include <fstream>
#include <map>
#include "SymbolBuffer.cpp"
#include "Types.h"
#include "Token.cpp"

#define SUCCESS 1
#define ERROR -1

using namespace std;

class Lexer
{
public:

	//Constructor
	Lexer()
	{
		this->output = NULL;
		this->lineNumber = 1;
		this->columnNumber = 1;
	}

	/*
	*	METHOD: openSource
	*
	*	DESCRIPTION: Attempts to open the source file. If the source file
	*	is successfully opened, then it also opens the destination file.
	*	In addition, if the source file and destination file are successfully
	*	opened, then the Lexer's private members (i.e. lineNumber, columnNumber
	*	curChar, and nextChar) are all initialized.
	*
	*	PRECONDITIONS: A file can not already be open.
	*
	*	POSTCONDITIONS: If successful in opening the source file and destination
	*	file, then it initializes lineNumber, columnNumber, curChar, and nextChar
	*	to begin reading from the file.
	*/

	int openSource(string inFile)
	{
		//if a file is already open, return ERROR.
		if (source.is_open())
		{
			cerr << "File already opened." << endl;
			return ERROR;
		}

		//Open file input by user
		source.open(inFile);

		//make sure source file was successfully opened.
		if (!source.is_open())
		{
			cerr << "Failed to open source file." << endl;
			source.close();
			return ERROR;
		}

		//check if file is empty
		if (source.peek() == EOF)
		{	
			cerr << "Source file is empty.";
			source.close();
			return ERROR;
		}
		else
		{
			//attempt opening destination file
			if ((this->output = freopen("Tokens.txt", "w", stderr)) == NULL)
			{
				cerr << "Could not open destination file." << endl;
				return ERROR;
			}
		}

		//At this point, we know the file is open and has at least one char.
		//initialize currentChar and nextChar
		source.get(curChar);

		if (!source.get(nextChar))
		{
			nextChar = '\0';
		}

		//intialize lineNumber and columnNumber
		lineNumber = columnNumber = 1;

		return SUCCESS;
	}

	/*
	*	METHOD: next
	*
	*	DESCRIPTION: Read a token from the source file one at a time. The general
	*	idea is that the lexer will continue to read in a single character at a
	*	time until it has determined that it has encountered a token. The body of
	*	the method contains a while loop with a nested switch statement. The while
	*	loop iterates so long as a token has not been found or the end of the file
	*	has not been reached. The switch statement determines the type of token
	*	that is currently being read.
	*
	*	PRECONDITIONS: File exists and is open.
	*
	*	POSTCONDITIONS: Updates global state: lineNumber, columnNumber, curChar
	*	and nextChar.
	*/

	//Get one token at a time.
	Token* next()
	{
		//stores a symbol string as it is read character by character.
		SymbolBuffer symBuf;
		Token *token = NULL;
		bool tokenFound = false;

		//loop getting single characters as long as the file lasts and as long
		//as a token has not been found.
		while (curChar != '\0' && !tokenFound)
		{
			//*****************************************************************
			//MAIN SWITCH
			//*****************************************************************
			switch (curChar)
			{
				//*****************************************************************
				//OPERATORS
				//*****************************************************************
				case '-':
				{
					token = new Token(MINUS, lineNumber, columnNumber);
					tokenFound == true;
					break;
				}
				case '!':
				{
					token = new Token(UNKNOWN, lineNumber, columnNumber);

					if (nextChar == '=')
					{
						token->setKind(INEQUALITY);
						getNextChar();
					}
					else
					{
						token->setKind(UNARY_NOT);
					}

					tokenFound == true;
					break;
				}
				case '&':
				{
					token = new Token(UNKNOWN, lineNumber, columnNumber);

					if (nextChar == '&')
					{
						token->setKind(LOGICAL_AND);
						getNextChar();
					}
					else
					{
						token->setKind(BITWISE_AND);
					}

					tokenFound = true;
					break;
				}
				case '*':
				{
					token = new Token(MULT, lineNumber, columnNumber);
					tokenFound = true;
					break;
				}
				case '/':
				{
					//The DIV operator can also signify the beginning of a comment,
					//therefore this case needs to include logic to determine whether
					//curChar marks the beginning of a comment.

					if (nextChar == '/')
					{ 	//read comment until the end of line char or end of file
						while (curChar != '\n' && curChar != '\0')
						{
							getNextChar();
						}

						lineNumber++;
						columnNumber = 0; //incremented at end of loop
						break;
					}
					else if (nextChar == '*')
					{ 	//read comment until '*/' is reached

						//Increment char pointers BEFORE while loop. If this operation
						//is not performed, then the string "/*/" may prematurely
						//exit the loop and fail to consume the full comment.
						getNextChar();

						while (true)
						{
							getNextChar();

							//check if the end of comment has been reached.
							if (curChar == '*' && nextChar == '/')
							{	//consume nextChar so that curChar will be set to
								//the character directly after nextChar at the
								//beginning of the next iteration of the main loop.
								getNextChar();
								break;
							}
							//check for new line in multi-line comment
							else if (curChar == '\n')
							{	//Update global.
								columnNumber = 1;
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
						token = new Token(DIV, lineNumber, columnNumber);
						tokenFound = true;
						break;
					}
				}
				case '^':
				{
					token = new Token(HAT, lineNumber, columnNumber);
					tokenFound = true;
					break;
				}
				case '|':
				{
					token = new Token(UNKNOWN, lineNumber, columnNumber);

					if (nextChar == '|')
					{	// "||"
						token->setKind(LOGICAL_OR);
						getNextChar();
					}
					else
					{	// "|"
						token->setKind(BITWISE_OR);
					}

					tokenFound = true;
					break;
				}
				case '~':
				{
					token = new Token(UNARY_TILDE, lineNumber, columnNumber);
					tokenFound = true;
					break;
				}
				case '+':
				{
					token = new Token(PLUS, lineNumber, columnNumber);
					tokenFound = true;
					break;
				}
				case '<':
				{
					token = new Token(UNKNOWN, lineNumber, columnNumber);

					if (nextChar == '<')
					{
						token->setKind(BITWISE_SHIFT_LEFT);
						getNextChar();
					}
					else if (nextChar == '=')
					{
						token->setKind(LESS_THAN_EQUAL);
						getNextChar();
					}
					else
					{
						token->setKind(LESS_THAN);
					}

					tokenFound = true;
					break;
				}
				case '>':
				{
					token = new Token(UNKNOWN, lineNumber, columnNumber);

					if (nextChar == '>')
					{
						token->setKind(BITWISE_SHIFT_RIGHT);
						getNextChar();
					}
					else if (nextChar == '=')
					{
						token->setKind(GREATER_THAN_EQUAL);
						getNextChar();
					}
					else
					{
						token->setKind(GREAT_THAN);
					}

					tokenFound = true;
					break;
				}
				case '=':
				{
					token = new Token(UNKNOWN, lineNumber, columnNumber);

					if (nextChar == '=')
					{
						token->setKind(EQUALITY);
						getNextChar();
					}
					else
					{
						token->setKind(ASSIGNMENT);
					}

					tokenFound = true;
					break;
				}


				//*****************************************************************
				//SINGLE CHARACTER TOKENS
				//*****************************************************************
				//new line
				case '\n':
				{
					//simply consume newline char.
					lineNumber++;
					columnNumber = 0; //incremented at end of main loop
					break;
				}
				case '(':
				{
					token = new Token(LEFT_PAREN, lineNumber, columnNumber);
					tokenFound = true;
					break;
				}
				case ')':
				{
					token = new Token(RIGHT_PAREN, lineNumber, columnNumber);
					tokenFound = true;
					break;
				}
				case ';':
				{
					token = new Token(SEMICOLON, lineNumber, columnNumber);
					tokenFound = true;
					break;
				}
				case '[':
				{
					token = new Token(LEFT_BRACKET, lineNumber, columnNumber);
					tokenFound = true;
					break;
				}
				case ']':
				{
					token = new Token(RIGHT_BRACKET, lineNumber, columnNumber);
					tokenFound = true;
					break;
				}
				case '{':
				{
					token = new Token(LEFT_BRACE, lineNumber, columnNumber);
					tokenFound = true;
					break;
				}
				case '}':
				{
					token = new Token(RIGHT_BRACE, lineNumber, columnNumber);
					tokenFound = true;
					break;
				}
				case ',':
				{
					token = new Token(COMMA, lineNumber, columnNumber);
					tokenFound = true;
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
					token = new Token(STRING_LITERAL, lineNumber, columnNumber);

					//consume '"'
					getNextChar();

					while (curChar != '"' && curChar != '\0')
					{
						symBuf.addChar(curChar);
						getNextChar();
					}

					//Syntax error if EOF reached before end of string.
					if (curChar == '\0')
					{
						token->setKind(INVALID);
						tokenFound = true;
						break;
					}

					string symbol = symBuf.getSymbol();
					token->setLiteral(symbol);

					//intern the string in symbol table
					symbolTable[symbol] = string(symbol);
					token->setSymbolPtr(&symbolTable[symbol]);

					tokenFound = true;
					break;
				}

				//*****************************************************************
				//CHARACTER LITERAL
				//*****************************************************************
				case '\'':
				{
					//Simply read the character literal until the matching single quote.
					token = new Token(CHAR_LITERAL, lineNumber, columnNumber);

					//consume '\'' (single quote)
					getNextChar();

					//NOTE: Character literals are represented by multiple characters
					while (curChar != '\'' && curChar != '\0')
					{
						symBuf.addChar(curChar);
						getNextChar();
					}

					//Syntax error if EOF reached before end of string.
					if (curChar == '\0')
					{
						token->setKind(INVALID);
						tokenFound = true;
						break;
					}

					token->setLiteral(symBuf.getSymbol());
					tokenFound = true;
					break;
				}

				//*****************************************************************
				//INTEGER OR FLOATING POINT NUMBER
				//*****************************************************************
				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9': case '.':
				{
					if (curChar == '.')
					{
						if (!isNumber(nextChar))
						{
							token = new Token(DOT, lineNumber, columnNumber);
							tokenFound = true;
							break;
						}
					}

					token = new Token(INT_LITERAL, lineNumber, columnNumber);
					bool hasDot = false;
					bool hasError = false;

					//Loop to get complete number
					while (true)
					{
						symBuf.addChar(curChar);

						if (curChar == '.')
						{
							if (hasDot == true)
							{
								cerr << "Syntax Error. Dot in wrong place" << endl;
								hasError = true;

								//Consume the rest of the invalid token
								while (nextChar != ' ' && nextChar != '\n' && nextChar != '\t')
								{
									getNextChar();
								}

								break;
							}

							token->setKind(FLOAT_LITERAL);
							hasDot = true;
						}

						//If there is more to the number
						if (isNumber(nextChar) || nextChar == '.')
						{
							getNextChar();
						}
						else //Number is completed
						{
							break;
						}
					}

					//Update and print token if it is valid
					if (!hasError)
					{
						token->setLiteral(symBuf.getSymbol());
						tokenFound = true;
					}

					break;
				}

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
					token = new Token(IDENTIFIER, lineNumber, columnNumber);

					//Loop to get complete identifier
					while (true)
					{
						if (!symBuf.addChar(curChar))
						{
							cerr << "Error: Identifier has more than 64 characters" << endl;
						}

						//If there is more to the identifier
						if (isLetter(nextChar) || isNumber(nextChar))
						{
							getNextChar();
						}
						else //Number is completed					
						{
							break;
						}
					}

					string ident = symBuf.getSymbol();
					token->setLiteral(ident);

					//If not a keyword, add identifier to symbol table
					if (!isKeyword(ident, *token))
					{
						symbolTable[ident] = string(ident);
						token->setLiteral(ident);
						token->setSymbolPtr(&symbolTable[ident]);
					}

					tokenFound = true;
					break;
				}

				//If char matches none of the cases, it is invalid
				default:
				{
					token = new Token(INVALID, lineNumber, columnNumber);
					tokenFound = true;
				}
			}

			//Advance to next character in source file.
			getNextChar();
		}

		return token;
	}

	/*
	*	METHOD: hasNext
	*
	*	DESCRIPTION: Check the source file for additional input.
	*
	*	PRECONDITIONS: File exists and is open.
	*/

	bool hasNext()
	{
		//curChar == '\n' and nextChar == '\0' is an edge case for EOF
		if ((curChar == '\n' && nextChar == '\0') || (curChar == '0'))
		{
			source.close();
			return false;
		}
		else
		{
			return true;
		}
	}

	/*
	*	METHOD: printSymbolTable
	*
	*	DESCRIPTION: Debuggin utility.
	*/

	void printSymbolTable()
	{
		for (map<string, string>::iterator it = symbolTable.begin(); it != symbolTable.end(); ++it)
		{
			cout << it->second << endl;
		}
	}

//*****************************************************************
//PRIVATE MEMBERS AND FUNCTIONS
//*****************************************************************
private:
	//GLOBALS
	char curChar;
	char nextChar;
	int lineNumber;
	int columnNumber;
	ifstream source;
	map <string, string> symbolTable;
	FILE *output;

	/*
	*	METHOD: isNumber
	*
	*	DESCRIPTION: Check if input char is a digit
	*/

	bool isNumber(char curChar)
	{
		if (curChar == '0' || curChar == '1' || curChar == '2' || curChar == '3' || curChar == '4' ||
			curChar == '5' || curChar == '6' || curChar == '7' || curChar == '8' || curChar == '9')
			return true;
		else
			return false;
	}

	/*
	*	METHOD: isLetter
	*
	*	DESCRIPTION: Check if input char is a valid letter or '_'
	*/

	bool isLetter(char curChar)
	{
		if (curChar == 'A' || curChar == 'B' || curChar == 'C' || curChar == 'D' || curChar == 'E' ||
			curChar == 'F' || curChar == 'G' || curChar == 'H' || curChar == 'I' || curChar == 'J' ||
			curChar == 'K' || curChar == 'L' || curChar == 'M' || curChar == 'N' || curChar == 'O' ||
			curChar == 'P' || curChar == 'Q' || curChar == 'R' || curChar == 'S' || curChar == 'T' ||
			curChar == 'U' || curChar == 'V' || curChar == 'W' || curChar == 'X' || curChar == 'Y' ||
			curChar == 'Z' || curChar == 'a' || curChar == 'b' || curChar == 'c' || curChar == 'd' ||
			curChar == 'e' || curChar == 'f' || curChar == 'g' || curChar == 'h' || curChar == 'i' ||
			curChar == 'j' || curChar == 'k' || curChar == 'l' || curChar == 'm' || curChar == 'n' ||
			curChar == 'o' || curChar == 'p' || curChar == 'q' || curChar == 'r' || curChar == 's' ||
			curChar == 't' || curChar == 'u' || curChar == 'v' || curChar == 'w' || curChar == 'x' ||
			curChar == 'y' || curChar == 'z' || curChar == '_')
			return true;
		else
			return false;
	}

	/*
	*	METHOD: isKeyword
	*
	*	DESCRIPTION: check if input string is a keyword. If so, 
	*	set the token Kind to this keyword.
	*/

	bool isKeyword(string word, Token &token)
	{
		if (word == "byte") token.setKind(BYTE);
		else if (word == "const") token.setKind(CONST);
		else if (word == "else") token.setKind(ELSE);
		else if (word == "end") token.setKind(END);
		else if (word == "exit") token.setKind(EXIT);
		else if (word == "float64") token.setKind(FLOAT64);
		else if (word == "for") token.setKind(FOR);
		else if (word == "function") token.setKind(FUNCTION);
		else if (word == "if") token.setKind(IF);
		else if (word == "int32") token.setKind(INT32);
		else if (word == "print") token.setKind(PRINT);
		else if (word == "record") token.setKind(RECORD);
		else if (word == "ref") token.setKind(REF);
		else if (word == "return") token.setKind(RETURN);
		else if (word == "static") token.setKind(STATIC);
		else if (word == "type") token.setKind(TYPE);
		else if (word == "var") token.setKind(VAR);
		else if (word == "while") token.setKind(WHILE);
		else return false;
		return true;
	}

	/*
	*	METHOD: getNextChar
	*
	*	DESCRIPTION: Helper method. Get the next char of the 
	*	input and nextChar. Increment curChar and columnNumber
	*/

	void getNextChar()
	{
		curChar = nextChar;

		if (!source.get(nextChar))
		{
			nextChar = '\0';
		}

		columnNumber++;
	}
};
