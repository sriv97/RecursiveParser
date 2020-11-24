/*
Sri Velagapudi
Project 3 
Recursive Descent Parser
*/
#include "parse.h"
#include "analyzer.cpp"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

inline bool Prog(istream& in, int& line) {
	cout << "in Prog" << endl;

	// in here, you want to check if there is a `begin` and `end` keyword

	string identstr;

	LexItem begin = Parser::GetNextToken(in, line);
	cout << begin.GetToken() << endl;
	// get the `begin` token
	if (begin.GetToken() != BEGIN) {
		return false;
	}
	line++;
	bool result = StmtList(in,line);




	//identstr = begin.GetLexeme();
	//line++;

	// validate stmtlist
	/*
	if (!StmtList(in[identstr.length()], line) {
		return false;
	}

	*/
	// get the `end` token at the very last line
	LexItem end = Parser::GetNextToken(in, line); // get the end token at the very last line (this line might be wrong)
		if (begin.GetToken() != END) {
			return false;
		}

	return result; // something like this to tell the user that this was a valid program (if all three bools are true)

	//check for begin, then call stmtlist, then check for end
}
inline bool StmtList(istream& in, int& line) {
	cout << "in StmtList" << endl;


	string identstr;

//	LexItem t = Parser::GetNextToken(in, line);

//	cout << t << endl;

	bool result = Stmt(in, line);
	LexItem t = Parser::GetNextToken(in,line);
	cout << "hello" << endl;
	cout << t.GetLexeme() << endl;
		Token a = t.GetToken();
		while(a == SCOMA){
			line++;
			result = Stmt(in,line);
			LexItem t = Parser::GetNextToken(in,line);
			a = t.GetToken();
		}	
	return result;

	//return false;

	//check for scoma in a loop

	//call stmt then check for SCOMA
	//next in a loop do the same above
}
inline bool Stmt(istream& in, int& line) {
	cout << "in Stmt" << endl;
	LexItem t = Parser::GetNextToken(in, line);
	Token b = t.GetToken();
	cout << b << endl;
	cout << t << endl;
	cout << Token::PRINT << endl;
	switch (b){
		case Token::PRINT:
			return PrintStmt(in,line);
		case Token::IDENT:
			return AssignStmt(in,line);
		case Token::IF:
			return IfStmt(in,line);
		default:
			ParseError(line,"Invalid statement");
	}
	//check for PRINT, then call PrintStmt
	//check for IDENT, then call AssignStmt
	//check for IF, then call IfStmt
}
inline bool PrintStmt(istream& in, int& line) {
	cout << "in PrintStmt" << endl;
	bool status = ExprList(in, line);
	if (!status) {
		ParseError(line, "Invalid expression after print");
		return false;
	}
	return status;
}
inline bool IfStmt(istream& in, int& line) {
	cout << "in IfStmt" << endl;
	//check for IF hen check LPAREN then call expr then checkfor RPAREN
	//check for THEN and call Stmt
}
inline bool AssignStmt(istream& in, int& line) {
	cout << "in AssignStmt" << endl;
	//call Var then check for EQ then call Expr
}
inline bool ExprList(istream& in, int& line) {
	cout << "in ExprList" << endl;
	bool result = Expr(in,line);
	Token a = t.GetToken();
		while(a == COMA){
			line++;
			result = Expr(in,line);
			LexItem t = Parser::GetNextToken(in,line);
			a = t.GetToken();
		}
	return result;
	//call Expr
	//in a loop check for COMA and call Expr
}
inline bool Expr(istream& in, int& line) {
	cout << "in Expr" << endl;
	bool result = Term(in,line);
	return result;
	//call Term
	//in a loop check for PLUS or MINUS and call Term
}
inline bool Term(istream& in, int& line) {
	cout << "in Term" << endl;
	bool result = Factor(in,line);
	return result;
	//call Factor
	//in a loop check for MULT or DIV and call Factor
}
inline bool Var(istream& in, int& line) {
	cout << "in Var" << endl;
	string identstr;

	LexItem t = Parser::GetNextToken(in, line);

	if (t.GetToken() == IDENT) {
		identstr = t.GetLexeme();
		if (!(defVar[identstr])) {
			defVar[identstr] = true;
		}
		return true;
	}
	return false;
}
inline bool Factor(istream& in, int& line) {
	cout << "in Factor" << endl;
	LexItem t = Parser::GetNextToken(in, line);
	Token b = t.GetToken();

	switch (b){
		case Token::IDENT: {
			string identstr = t.GetLexeme();
			if (!(defVar[identstr])) {
				defVar[identstr] = true;
			}
			break;
		}case Token::ICONST:{
			cout << "iconst" << endl;
			break;
		}case Token::RCONST:{
			cout << "rconst" << endl;
			break;
		}case Token::SCONST:{
			cout << "sconst" << endl;
			break;
		}default:{
			Token a = t.GetToken();
			if(a != LPAREN){
				LexItem ex = Parser::GetNextToken(in,line);
				bool result = Expr(in,line);
				LexItem b = Parser::GetNextToken(in,line);
				if(b.GetToken() != RPAREN){
					ParseError(line,"Invalid statement");
				}
			}else {
				ParseError(line,"Invalid statement");
			}
	}
	}
	//check for IDENT
	//check for ICONST
	//check for RCONST
	//check for SCONST
	//check for LPAREN then call expr then check for RPAREN
	return true;
}

int main(int argc, char const* argv[]) {
	/* check if file can open and
	if there are more then one files
	provided.
	*/

	if (argc > 2) {
		std::cout << "ONLY ONE FILE NAME ALLOWED" << std::endl;
		return -1;
	}

	std::filebuf fb;
	if (!(fb.open(argv[1], std::ios::in))) {
		std::cout << "CANNOT OPEN THE FILE " << argv[1] << std::endl;
		return -2;
	}

	std::istream myfile(&fb);
	int number = 1;
	bool final = Prog(myfile, number);
	if (final) {
		std::cout << "Successful Parsing" << std::endl;
	}
	else {
		std::cout << "Unsuccessful Parsing " << std::endl;
		std::cout << "Number of Syntax Errors: " << error_count << std::endl;
	}
}
