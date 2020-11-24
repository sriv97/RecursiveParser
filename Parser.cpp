//Parser.cpp

#include "parse.h"
#include "Analyzer.cpp" //aka Lex.cpp
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
/*
check the slides for more information they are very helpful.
Try not to touch PrintStmt or Var because those where given in the slides.

Otherwise, Good Luck!!!
*/
inline bool Prog(istream& in, int& line){
  cout<<"in Prog"<<endl; // these are only for debuging remove all of them after
  
  // in here, you want to check if there is a `begin` and `end` keyword
  
  string identstr;

  LexItem begin = Parser::GetNextToken(in,line);
	// get the `begin` token
  if(begin.GetToken() != BEGIN){
    return false;
  }
  
  identstr=begin.GetLexeme();
  line++;
  
  // validate stmtlist
  if (!StmtList(in[identstr.length()], line) {
  	return false; 
  }

  // get the `end` token at the very last line
  LexItem end = Parser::GetNextToken(in, line); // get the end token at the very last line (this line might be wrong)
  if(begin.GetToken() != END){
    return false;
  }
  
  return true; // something like this to tell the user that this was a valid program (if all three bools are true)
  
  //check for begin, then call stmtlist, then check for end
}
inline bool StmtList(istream& in, int& line){
  cout<<"in StmtList"<<endl;
  
  
  string identstr;

  LexItem t = Parser::GetNextToken(in,line);
	
  return Stmt(in, line);
  
  
  
  return false;
  
  
  LexItem a = GetNextToken(in, line);
  if a == "{" {
    return Stmt(in, line+1);
  } else {
    return Stmt(in, line+1); 
  }
  //call stmt then check for SCOMA
  //next in a loop do the same above
}
inline bool Stmt(istream& in, int& line){
  cout<<"in Stmt"<<endl;
  //check for PRINT, then call PrintStmt
  //check for IDENT, then call AssignStmt
  //check for IF, then call IfStmt
}
inline bool PrintStmt(istream& in, int& line){
  cout<<"in PrintStmt"<<endl;
  bool status = ExprList(in, line);
  if(!status){
    ParseError(line, "Invalid expression after print");
    return false;
  }
  return status;
}
inline bool IfStmt(istream& in, int& line){
  cout<<"in IfStmt"<<endl;
  //check for IF hen check LPAREN then call expr then checkfor RPAREN
  //check for THEN and call Stmt
}
inline bool AssignStmt(istream& in, int& line){
  cout<<"in AssignStmt"<<endl;
  //call Var then check for EQ then call Expr
}
inline bool ExprList(istream& in, int& line){
  cout<<"in ExprList"<<endl;
  //call Expr
  //in a loop check for COMA and call Expr
}
inline bool Expr(istream& in, int& line){
  cout<<"in Expr"<<endl;
  //call Term
  //in a loop check for PLUS or MINUS and call Term
}
inline bool Term(istream& in, int& line){
  cout<<"in Term"<<endl;
  //call Factor
  //in a loop check for MULT or DIV and call Factor
}
inline bool Var(istream& in, int& line){
  cout<<"in Var"<<endl;
  string identstr;

  LexItem t = Parser::GetNextToken(in,line);
	
  if(t.GetToken() == IDENT){
    identstr=t.GetLexeme();
    if(!(defVar[identstr])){
      defVar[identstr]=true;
    }
    return true;
  }
  return false;
}
inline bool Factor(istream& in, int& line){
  cout<<"in Factor"<<endl;
  //check for IDENT
  //check for ICONST
  //check for RCONST
  //check for SCONST
  //check for LPAREN then call expr then check for RPAREN
}

int main(int argc, char const *argv[]) {
  /* check if file can open and
  if there are more then one files
  provided.
  */
  int number = 1;
  bool final = Prog(myfile,number);
  if (final){
    std::cout<<"Successful Parsing"<<std::endl;
  }
  else{
    std::cout<<"Unsuccessful Parsing "<<std::endl;
    std::cout<<"Number of Syntax Errors: "<< error_count<<std::endl;
  }
}
