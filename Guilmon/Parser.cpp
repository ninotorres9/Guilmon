
#include "stdafx.h"
#include "Parser.h"

namespace Guilmon {
	Token Scanner::getToken()
	{
		skipSpace();
		auto firstChar = peekChar();
		if (isdigit(firstChar)) {
			return getNumberToken();
		}
		else if (firstChar == '%') {
			return getVariableToken();
		}
		else {
			return getKeywordToken();
		}
	}
	Instruction Parser::expression() {
		auto firstToken = scanner_.peek().value();
		if (firstToken == "push" || firstToken == "store" 
			|| firstToken == "jmp" || firstToken == "jz") {
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

