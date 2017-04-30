#ifndef IDENTIFIER_BUFFER
#define IDENTIFIER_BUFFER

#include <iostream>
#include <string.h>

#define MAX_IDENTIFIER_SIZE 64

using namespace std;

class SymbolBuffer
{
private:
	int currentPos;
	char buffer[MAX_IDENTIFIER_SIZE + 1];

public:

	SymbolBuffer()
	{
		clear(); //initializes object
	}

	//Simply adds another character to the buffer
	bool addChar(char c)
	{
	//make sure there is room in the buffer
	if (currentPos < MAX_IDENTIFIER_SIZE)
	{
		buffer[currentPos] = c;
		currentPos++;
		return true;
	}

	return false;
	}

	//returns the null terminated string of characters 
	//stored in buffer as a string object.
	string getSymbol()
	{
		string str(buffer);
		this->clear();
		return str;
	}
		  
private:

	//Clears the buffer and resets counter
	void clear()
	{
		bzero(buffer, MAX_IDENTIFIER_SIZE + 1);
		currentPos = 0;
	}
};

#endif