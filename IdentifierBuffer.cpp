#ifndef IDENTIFIER_BUFFER
#define IDENTIFIER_BUFFER

#include <string.h>
#include <iostream>

#define MAX_IDENTIFIER_SIZE 64

using namespace std;

class IdentifierBuffer
{
  private: int currentPos;
  private: char buffer[MAX_IDENTIFIER_SIZE + 1];

  public: IdentifierBuffer()
  {
    clear(); //initializes object
  }

  //Clears the buffer and resets counter
  private: void clear()
  {
    bzero(buffer, MAX_IDENTIFIER_SIZE + 1);
    currentPos = 0;
  }

  //Simply adds another character to the buffer
  public: bool addChar(char c)
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
  public: string getIdentifier()
  {
    string str(buffer);
	this->clear();
	return str;
  }
};

#endif