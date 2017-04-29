#include <iostream>
#include <fstream>
#include "IdentifierBuffer.cpp"
#include "Types.h"
#include "Token.cpp"
#define SUCCESS 1
#define ERROR -1

using namespace std;

class Lexer
{
private:
	//GLOBALS
	char curChar;
	char nextChar;
	int lineNumber;
	int columnNumber;
	//Temp buffer for storing strings.
	IdentifierBuffer idBuf;	
	ifstream source;
	FILE *output;
	
public:
	//Constructor
	Lexer()
	{
		this->output = NULL;
		lineNumber = 1;
		columnNumber = 0;
		curChar = '\0';
		nextChar = '\0';
	}

	int openSource(string inFile)
	{
		//Open file input by user
		source.open(inFile.c_str());

		if (!source.is_open())
		{
			cerr << "Failed to open source file." << endl;
			source.close();
			return ERROR;
		}

		int firstChar = source.peek();

		if (firstChar == EOF)
		{
			cerr << "Source file is empty.";
			source.close();
			return ERROR;
		}
		else
		{
			//only open output if source is nonempty
			if ((this->output = freopen("Tokens.txt", "w", stderr)) == NULL)
			{
				cerr << "Failed to open output file." << endl;
				return ERROR;
			}
		}

		return SUCCESS;
	}
	
	int closeSource()
	{
		source.close();
	}

	//Main lexing function
	Token getNextToken()
	{
		//If this is the first time this function has been called, check if empty.
		if(curChar == '\0')
		{	
			if (!source.get(curChar)) 
			{
				cerr << "Source file is empty." << endl;
				Token token(UNKNOWN, lineNumber, columnNumber);
				return token;
			}
			nextChar = curChar; //Set both to first char of input.
		}
		
		//loop getting single characters as long as the file lasts.
		while (curChar != '\0')
		{
			getNextChar();
			if(curChar == '\0')
				break;
//			cerr << "curr char is: " << curChar << " and next char is: " << nextChar << endl;
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
					Token token(MINUS, lineNumber, columnNumber);
					return token;
				}
				case '!':
				{
					Token token(UNKNOWN, lineNumber, columnNumber);

					if (nextChar == '=')
					{
						token.setKind(INEQUALITY);
						getNextChar();
					}
					else
					{
						token.setKind(UNARY_NOT);
					}
					return token;
				}
				case '&':
				{
					Token token(UNKNOWN, lineNumber, columnNumber);

					if (nextChar == '&')
					{
						token.setKind(LOGICAL_AND);
						getNextChar();
					}
					else
					{
						token.setKind(BITWISE_AND);
					}
					return token;
				}
				case '*':
				{
					Token token(MULT, lineNumber, columnNumber);
					return token;
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
						return token;
					}
				}
				case '^':
				{
					Token token(HAT, lineNumber, columnNumber);
					return token;
				}
				case '|':
				{
					Token token(UNKNOWN, lineNumber, columnNumber);

					if (nextChar == '|')
					{	// "||"
						token.setKind(LOGICAL_OR);
						getNextChar();
					}
					else
					{	// "|"
						token.setKind(BITWISE_OR);
					}
					return token;
				}
				case '~':
				{
					Token token(UNARY_TILDE, lineNumber, columnNumber);
					return token;
				}
				case '+':
				{
					Token token(PLUS, lineNumber, columnNumber);
					return token;
				}
				case '<':
				{
					Token token(UNKNOWN, lineNumber, columnNumber);

					if (nextChar == '<')
					{
						token.setKind(BITWISE_SHIFT_LEFT);
						getNextChar();
					}
					else if (nextChar == '=')
					{
						token.setKind(LESS_THAN_EQUAL);
						getNextChar();
					}
					else
					{
						token.setKind(LESS_THAN);
					}

					return token;
				}
				case '>':
				{
					Token token(UNKNOWN, lineNumber, columnNumber);

					if (nextChar == '>')
					{
						token.setKind(BITWISE_SHIFT_RIGHT);
						getNextChar();
					}
					else if (nextChar == '=')
					{
						token.setKind(GREATER_THAN_EQUAL);
						getNextChar();
					}
					else
					{
						token.setKind(GREATER_THAN);
					}

					return token;
				}
				case '=':
				{
					Token token(UNKNOWN, lineNumber, columnNumber);

					if (nextChar == '=')
					{
						token.setKind(EQUALITY);
						getNextChar();
					}
					else
					{
						token.setKind(ASSIGNMENT);
					}

					return token;
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
					return token;
				}
				case ')':
				{
					Token token(RIGHT_PAREN, lineNumber, columnNumber);
					return token;
				}
				case ';':
				{
					Token token(SEMICOLON, lineNumber, columnNumber);
					return token;
				}
				case '[':
				{
					Token token(LEFT_BRACKET, lineNumber, columnNumber);
					return token;
				}
				case ']':
				{
					Token token(RIGHT_BRACKET, lineNumber, columnNumber);
					return token;
				}
				case '{':
				{
					Token token(LEFT_BRACE, lineNumber, columnNumber);
					return token;
				}
				case '}':
				{
					Token token(RIGHT_BRACE, lineNumber, columnNumber);
					return token;
				}
				case ',':
				{
					Token token(COMMA, lineNumber, columnNumber);
					return token;
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
					getNextChar();

					while (curChar != '"')
					{
						idBuf.addChar(curChar);
						getNextChar();
					}

					//TODO: Store string inside a data structure.
					token.setLiteral(idBuf.getIdentifier());
					return token;
				}

				//*****************************************************************
				//CHARACTER LITERAL
				//*****************************************************************
				case '\'':
				{
					//Simply read the character literal until the matching single quote.
					Token token(CHAR_LITERAL, lineNumber, columnNumber);

					//consume '\'' (single quote)
					getNextChar();

					//NOTE: Character literals are represented by multiple characters
					//in source code.
					while (curChar != '\'')
					{
						idBuf.addChar(curChar);
						getNextChar();
					}

					token.setLiteral(idBuf.getIdentifier());
					return token;
				}

				//*****************************************************************
				//INTEGER OR FLOATING POINT NUMBER
				//*****************************************************************
				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9': case '.':
				{
					if (curChar == '.')
					{
						if (isNumber(nextChar) == false)
						{
							Token token(DOT, lineNumber, columnNumber);
							return token;
							break;
						}
					}

					Token token(INT_LITERAL, lineNumber, columnNumber);
					IdentifierBuffer numBuffer;
					bool hasDot = false;
					bool hasError = false;

					//Loop to get complete number
					while (true)
					{
						numBuffer.addChar(curChar);

						if (curChar == '.')
						{
							if (hasDot == true)
							{
								cerr << "Syntax Error. Dot in wrong place" << endl;
								hasError = true;

								//NOTE: Not sure if this is valid. Check specification
								// for numbers.

								//Consume the rest of the invalid token
								while (nextChar != ' ' && nextChar != '\n' && nextChar != '\t')
								{
									getNextChar();
									//								if(!source.get(nextChar))
									//									break;
								}
								break;
							}

							token.setKind(FLOAT_LITERAL);
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
						token.setLiteral(numBuffer.getIdentifier());
						return token;
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
					Token token(IDENTIFIER, lineNumber, columnNumber);
					IdentifierBuffer idBuffer;

					//Loop to get complete identifier
					while (true)
					{
						if (idBuffer.addChar(curChar) == false)
							cerr << "Error: Identifier has more than 64 characters" << endl;

						//If there is more to the identifier
						if (isLetter(nextChar) || isNumber(nextChar))
							getNextChar();
						else //Number is completed					
							break;
					}

					string ident = idBuffer.getIdentifier();
					if (isKeyword(ident, token) == false)
						token.setLiteral(ident);
					return token;
				}

				//If char matches none of the cases, it is invalid
				default:
				{
					cerr << "invalid token" << endl;
				}

//				curChar = nextChar;
//				columnNumber++;
			}
		}
		Token token(UNKNOWN, -2, -2);
		return token;
		
		//close file and exit program
		//source.close();
	}

//*****************************************************************
//PRIVATE MEMBERS AND FUNCTIONS
//*****************************************************************
private:
	

	//Check if input char is a digit
	bool isNumber(char curChar)
	{
		if (curChar == '0' || curChar == '1' || curChar == '2' || curChar == '3' || curChar == '4' ||
			curChar == '5' || curChar == '6' || curChar == '7' || curChar == '8' || curChar == '9')
			return true;
		else
			return false;
	}

	//Check if input char is a valid letter or '_'
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

	//check if input string is a keyword. If so, set the token Kind to this keyword.
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

	//Get the next char of the input and nextChar. Increment curChar and columnNumber
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
