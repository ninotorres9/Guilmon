
#include "stdafx.h"
#include "Parser.h"

namespace Guilmon {
	Token Scanner::getToken()
	{
		Token token(TokenType::END, "end");

		skipSpace();
		auto firstChar = peekChar();
		if (isdigit(firstChar)) {
			token =  getNumberToken();
		}
		else if (firstChar == '%') {
			token = getVariableToken();
		}
		else if (firstChar == '@') {
			getChar();	// eat @
			token = Token(TokenType::FUNCTION, { getIdentifier() });
		}
		else {
			token = getKeywordToken();
		}
		skipSpace();
		return token;
	}
	Instruction Parser::expression() {
		auto firstToken = scanner_.peek().value();
		if (firstToken == "push" || firstToken == "store" 
			|| firstToken == "jmp"|| firstToken == "jz"   
			|| firstToken == "jnz" || firstToken == "assign"
			|| firstToken == "tag" || firstToken == "call"){
			; // one argument : push
			auto op = scanner_.get().value();
			auto arg = scanner_.get();
			return Instruction(op, { arg });
		}
		else {
			; // no argument : add sub ...
			auto op = scanner_.get().value();
			return Instruction(op, {});
		}
	}
}

