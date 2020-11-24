/*
Sri Velagapudi
Project 2 
Lexical Analyzer
*/
#include <iostream>
#include <regex>
#include <map>
#include "lex.h"

using namespace std;

static map <Token, string> tokenPrint = {
{PRINT,"PRINT"},
{IF,"IF"},
{BEGIN,"BEGIN"},
{END,"END"},
{IDENT,"IDENT"},
{ICONST,"ICONST"},
{SCONST,"SCONST"},
{RCONST,"RCONST"},
{PLUS,"PLUS"},
{MINUS,"MINUS"},
{MULT,"MULT"},
{DIV,"DIV"},
{EQ,"EQ"},
{LPAREN,"LPAREN"},
{RPAREN,"RPAREN"},
{SCOMA,"SCOMA"},
{COMA,"COMA"},
{ERR, "ERR"},
{DONE,"DONE"}
};

ostream& operator<<(ostream& out, const LexItem& tok){
Token t = tok.GetToken();
out << tokenPrint[t];
if(t == IDENT || t == ICONST || t == SCONST || t == RCONST || t == ERR){
out << " (" << tok.GetLexeme() << ")";
}
out << " Line #: " << tok.GetLinenum() << "\n";
return out;
}

LexItem currentToken;
LexItem previousToken;

LexItem getNextToken(istream & in , int & linenum) {
  enum TokenState {
    START,
    INID,
    INSTRING,
    ININT,
    INREAL,
    INCOMMENT,
    SIGN
  }
  lexstate = START;
  string lexeme;
  char character;
  char nextCharacter;

  while ( in .get(character)) {
    switch (lexstate) {
    case START:
      if (character == '\n')
        linenum++;

      if ( in .peek() == -1) {
        if (previousToken.GetToken() != END)
          return LexItem(ERR, "No END Token", previousToken.GetLinenum());
        return LexItem(DONE, lexeme, linenum);
      }

      if (isspace(character))
        continue;

      lexeme = character;

      if (character == '/' && char( in .peek()) == '/') {
        lexstate = INCOMMENT;
        continue;
      }

      if (character == '+' || character == '-' || character == '*' ||
        character == '/' || character == '(' || character == ')' ||
        character == '=' || character == ',' || character == ';') {
        lexstate = SIGN;
        continue;
      }

      if (character == '\"') {
        lexstate = INSTRING;
        continue;
      }

      if (isdigit(character)) {
        lexstate = ININT;
        continue;
      }

      if (character == '.') {
        lexstate = INREAL;
        continue;
      }

      if (isalpha(character)) {
        lexstate = INID;
        continue;
      }

      return LexItem(ERR, lexeme, linenum);

    case INID:
      if (regex_match(lexeme + character, regex("[a-zA-Z][a-zA-Z0-9]*")))
        lexeme += character;
      if ( in .peek() == -1 || !regex_match(lexeme + character, regex("[a-zA-Z][a-zA-Z0-9]*"))) {
        lexstate = START; in .putback(character);

        if (lexeme == "begin") {
          if (previousToken.GetToken() != ERR)
            return LexItem(ERR, lexeme, linenum);
          currentToken = LexItem(BEGIN, lexeme, linenum);
        } else if (lexeme == "print"){
          currentToken = LexItem(PRINT, lexeme, linenum);
        }else if (lexeme == "end") {
          if (previousToken.GetToken() != SCOMA)
            return LexItem(ERR, previousToken.GetLexeme(), linenum);
          currentToken = LexItem(END, lexeme, linenum);
        } else if (lexeme == "if")
          currentToken = LexItem(IF, lexeme, linenum);
        else if (lexeme == "then")
          currentToken = LexItem(THEN, lexeme, linenum);
        else {
          if (previousToken.GetToken() == IDENT)
            return LexItem(ERR, lexeme, linenum);
          currentToken = LexItem(IDENT, lexeme, linenum);
        }

        if (currentToken != BEGIN && previousToken == ERR)
          return LexItem(ERR, "No BEGIN Token", currentToken.GetLinenum());
        previousToken = currentToken;
        return currentToken;
      }
      break;

    case INSTRING:
      if (previousToken == ERR)
        return LexItem(ERR, "No Begin Token", linenum);
      if (character == 10)
        return LexItem(ERR, lexeme, linenum);

      if (regex_match(lexeme + character, regex("\"[ -~]*"))) {
        if (character == '\\' && in .peek() == '\"') {
          lexeme += character; in .get(character);
          lexeme += character;
          continue;
        } else
          lexeme += character;
      }

      if (regex_match(lexeme + character, regex("\"[ -~]*\""))) {
        lexstate = START;
        currentToken = LexItem(SCONST, lexeme, linenum);
        previousToken = currentToken;
        return currentToken;
      }
      break;

    case ININT:
      if (previousToken == ERR)
        return LexItem(ERR, "No Begin Token", linenum);
      if (isalpha(character))
        return LexItem(ERR, lexeme + character, linenum);
      if (regex_match(lexeme + character, regex("[0-9]+"))) {
        lexeme += character;
      }
      else if (character == '.') {
        lexstate = INREAL; in .putback(character);
        continue;
      } else {
        lexstate = START; in .putback(character);
        currentToken = LexItem(ICONST, lexeme, linenum);
        previousToken = currentToken;
        return currentToken;
      }
      break;

    case INREAL:
      if (previousToken == ERR)
        return LexItem(ERR, "No Begin Token", linenum);
      if (isalpha(character))
        return LexItem(ERR, lexeme + character, linenum);
      if (regex_match(lexeme + character, regex("[0-9]*\\.[0-9]+"))) {
        lexeme += character;
      } else if (regex_match(lexeme + character, regex("[0-9]*\\.[0-9]*"))) {
        lexeme += character;
      } else {
        if (lexeme[lexeme.length() - 1] == '.')
          return LexItem(ERR, lexeme, linenum);
        lexstate = START; in .putback(character);
        currentToken = LexItem(RCONST, lexeme, linenum);
        previousToken = currentToken;
        return currentToken;
      }
      break;

    case INCOMMENT:
      if (character == '\n') {
        linenum++;
        lexstate = START;
      }
      continue;

    case SIGN:
      if (previousToken == ERR)
        return LexItem(ERR, "No Begin Token", linenum);
      if (lexeme == "+" || lexeme == "*" || lexeme == "/") {
        Token token = previousToken.GetToken();
        if (token == IDENT || token == ICONST || token == RCONST) {
          lexstate = START; in .putback(character);
          if (lexeme == "+")
            currentToken = LexItem(PLUS, lexeme, linenum);
          else if (lexeme == "*")
            currentToken = LexItem(MULT, lexeme, linenum);
          else
            currentToken = LexItem(DIV, lexeme, linenum);
          previousToken = currentToken;
          return currentToken;
        } else
          return LexItem(ERR, lexeme + character, linenum);
      }
      if (lexeme == "-") {
        Token token = previousToken.GetToken();
        if (token == IDENT || token == ICONST || token == RCONST || token == EQ) {
          lexstate = START; in .putback(character);
          currentToken = LexItem(MINUS, lexeme, linenum);
          previousToken = currentToken;
          return currentToken;
        } else
          return LexItem(ERR, lexeme + character, linenum);
      }
      if (lexeme == "(") {
        Token token = previousToken.GetToken();
        if (token == IF || token == EQ || token == PLUS || token == MINUS ||
          token == MULT || token == DIV) {
          lexstate = START; in .putback(character);
          currentToken = LexItem(LPAREN, lexeme, linenum);
          previousToken = currentToken;
          return currentToken;
        } else
          return LexItem(ERR, lexeme + character, linenum);
      }
      if (lexeme == ")") {
        Token token = previousToken.GetToken();
        if (token == ICONST || token == RCONST || token == IDENT) {
          lexstate = START; in .putback(character);
          currentToken = LexItem(RPAREN, lexeme, linenum);
          previousToken = currentToken;
          return currentToken;
        } else
          return LexItem(ERR, lexeme + character, linenum);
      }
      if (lexeme == "=") {
        Token token = previousToken.GetToken();
        if (token == IDENT) {
          lexstate = START; in .putback(character);
          currentToken = LexItem(EQ, lexeme, linenum);
          previousToken = currentToken;
          return currentToken;
        } else
          return LexItem(ERR, lexeme + character, linenum);
      }
      if (lexeme == ",") {
        Token token = previousToken.GetToken();
        if (token == SCONST) {
          lexstate = START; in .putback(character);
          currentToken = LexItem(COMA, lexeme, linenum);
          previousToken = currentToken;
          return currentToken;
        } else
          return LexItem(ERR, lexeme + character, linenum);
      }
      if (lexeme == ";") {
        Token token = previousToken.GetToken();
        if (token == SCONST || token == ICONST || token == RCONST || token == IDENT) {
          lexstate = START; in .putback(character);
          currentToken = LexItem(SCOMA, lexeme, linenum);
          previousToken = currentToken;
          return currentToken;
        } else
          return LexItem(ERR, lexeme + character, linenum);
      }
      break;
    }
  }
  return LexItem(DONE, "", linenum);
}