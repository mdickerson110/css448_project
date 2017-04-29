#include <iostream>
#include <fstream>
#include "Lexer.cpp"
#include "Types.h"

using namespace std;
class Parser
{
	private:
	Token token;
	Lexer lex;
	
	public:	
	//main parse function
	int parse(string inFile)
	{	
	
		if (lex.openSource(inFile) == ERROR)
		{
			return -1;
		}	

		/* 
		//Print out Token test. Uncomment this to print out all tokens from Lexer
		Token token = lex.getNextToken();
		while(token.getColumnNumber() != -1)
		{
			token.printOut();
			token = lex.getNextToken();		
		}	
		*/
	
		token = lex.getNextToken();	
		program();
		lex.getNextToken();
		if(token.lineNumber == -2)
			cerr << "Parse Successful!" << endl;
	}
	
	private:
	
	void match(Kind kind)
	{
		if(token.kind == kind)
		{
			token.printOut(); //For testing purposes
			token = lex.getNextToken();
		}	
		else
		{
			cerr << "parse error" << endl;
		}	
	}
	
	//program -> statement*
	void program()
	{
		cerr << "program" << endl; // for testing purposes
		
		//TODO: this is a temporary way of doing multiple statments
		for(int i = 0; i < 20; i++)
		{
			statement();
		}
	}

	//statement -> declaration-statement | block-statement | for-statement | while-statement
	//			 | if-statement | print-statement | return-statement | exit-statement | expression-statement
	void statement()
	{
		cerr << "statement" << endl; // for testing purposes
		
		if(token.kind == TYPE || token.kind == FUNCTION || token.kind == CONST ||
			token.kind == IDENTIFIER)
		{
			declaration_statement();
		}
		else if(token.kind == LEFT_BRACE)
		{
			block_statement();
		}			
		else if(token.kind == FOR)
		{
			for_statement();
		}
		else if(token.kind == IF)
		{
			if_statement();
		}
		else if(token.kind == PRINT)
		{
			print_statement();
		}
		else if(token.kind == RETURN)
		{
			return_statement();
		}
		else if(token.kind == EXIT)
		{
			exit_statement();
		}
		//TODO: add case for statement -> expresssion
		//TODO: add error if no matches
	}
	
	//declaration-statement -> type-declaration | function-declaration | variable-declaration
	void declaration_statement(){
		cerr << "declaration-statement" << endl; // for testing purposes
		if(token.kind == TYPE)
		{
			type_declaration();
		}
		else if(token.kind == FUNCTION)
		{
			function_declaration();
		}
		else if (token.kind == STATIC || token.kind == CONST || token.kind == IDENTIFIER)
		{
			variable_declaration();
		}
		//TODO: Add error if none of these matches
	}
	
	//type-declaration -> type identifier type-descriptor ;
	void type_declaration()	
	{
		cerr << "type-declaration" << endl; // for testing purposes
		match(TYPE);
		match(IDENTIFIER);
		type_descriptor();
	}
	
	//type-descriptor -> non-array-type-descriptor | non_array_type_descriptor dimension
	//TODO: Common prefix???
	void type_descriptor()
	{
		cerr << "type-descriptor" << endl; // for testing purposes
		non_array_type_descriptor();
		if(token.kind == LEFT_BRACE)
		{
			dimension();
		}
	}
	
	//non-array-type-descriptor -> record-descriptor | identifier | basic-type
	void non_array_type_descriptor()
	{
		cerr << "non_array_type_descriptor" << endl; // for testing purposes
		if(token.kind == RECORD)
		{
			record_descriptor();
		}
		else if(token.kind == IDENTIFIER)
		{
			match(IDENTIFIER);
		}
		else if(token.kind == BYTE || token.kind == INT32 || token.kind == FLOAT64)
		{
			basic_type();
		}
	}
	
	//record-descriptor -> record field-declarations end
	void record_descriptor()
	{
		cerr << "record-descriptor" << endl; // for testing purposes
		match(RECORD);
		field_declarations();
		match(END);
	}
	
	void field_declarations()
	{
		
	}
	
	void field_declaration()
	{
		
	}
	
	//basic-type -> byte | int32 | float64
	void basic_type()
	{
		cerr << "basic_type" << endl; // for testing purposes
		if(token.kind == BYTE)
		{
			match(BYTE);
		}
		else if (token.kind == INT32)
		{
			match(INT32);
		}
		else if(token.kind == FLOAT64)
		{
			match(FLOAT64);
		}
		//TODO: add error for what happens if none of them match
	}
	
	//dimension -> [ expressions ]
	void dimension()
	{
		match(LEFT_BRACE);
		expressions();
		match(RIGHT_BRACE);
	}

	//function-declaration -> function identifier ( parametersopt ) type-descriptoropt block-statement
	void function_declaration()
	{
		match(FUNCTION);
		match(LEFT_PAREN);
		if(token.kind != RIGHT_PAREN)
		{
			parameters();
		}
		match(RIGHT_PAREN);
		if(token.kind != LEFT_BRACE)
		{
			type_descriptor();
		}
		block_statement();		
	}
	
	void parameters()
	{
		
	}
	
	void parameter()
	{
		
	}
	
	void dimension_wildcards()
	{
		
	}
	
	//variable-declaration -> static(opt) const(opt) var identifier type-descriptor ;
	//					 -> static(opt) const(opt) var identifier = expression ;
	void variable_declaration()
	{
		if(token.kind == STATIC)
		{
			match(STATIC);
		}
		if(token.kind == CONST)
		{
			match(CONST);
		}
		match(VAR);
		match(IDENTIFIER);
		if(token.kind == ASSIGNMENT)
		{
			expression();
		}
		else if(token.kind == RECORD || token.kind == IDENTIFIER || token.kind == BYTE || 
			token.kind == INT32 || token.kind == FLOAT64)
		{
			type_descriptor();
		}
		else
		{
			cerr << "parse error" << endl; //temporary error
		}
	}
	
	void block_statement()
	{
		
	}
	
	void for_statement()
	{
		cerr << "for statement" << endl;
		match(FOR);
	}
	
	void while_statement()
	{
		
	}
	
	void if_statement()
	{
		
	}
	
	void print_statement()
	{
			
	}
	
	void return_statement()
	{
		
	}
	
	void exit_statement()
	{
		
	}
	
	void expression_statement()
	{
		
	}
	
	void expressions()
	{
		
	}
	
	void expression()
	{
		
	}
	
	void type_cast()
	{
		
	}
	
	void function_call()
	{
		
	}
	
	void variable ()
	{
		
	}
	
	void subscript()
	{
		
	}
	
	void number()
	{
		
	}


};

