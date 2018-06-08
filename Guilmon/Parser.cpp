
#include "stdafx.h"
#include "Parser.h"

namespace Guilmon {
	Token Scanner::getToken()
	{
		Token token(TokenType::END, "end");

		skipSpace();
		auto firstChar = peekChar();
		if (isdigit(firstChar)) {
			token = getNumberToken();
		}
		else if (firstChar == '%') {
			token = getVariableToken();
		}
		else if (firstChar == '@') {
			getChar();	// eat @
			token = Token(TokenType::TAG, { getIdentifier() });
		}
		else if (firstChar == '\'') {
			getChar();	// eat '
			token = Token(TokenType::CHAR, { std::string(1, getChar()) });
			getChar();	// eat '
		}
		else if (firstChar == '&') {
			token = Token(TokenType::REFER, std::string(1, getChar()));
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
			|| firstToken == "jmp" || firstToken == "jz"
			|| firstToken == "jnz" || firstToken == "assign"
			|| firstToken == "tag" || firstToken == "call"
			|| firstToken == "free" || firstToken == "class") {
			; 
			return getOneArgumentInstruction();
		}
		else if (firstToken == "create_class") {
			return getTwoArgumentInstruction();
		}
		else {
			return getNoArgumentInstruction();
		}
	}
}

