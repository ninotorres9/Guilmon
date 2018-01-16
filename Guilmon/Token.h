#pragma once

#include <string>

namespace Guilmon {
	enum class TokenType
	{
		KEYWORD,
		NUMBER,
		VARIABLE,
		FUNCTION,
		END,
	};

	class Token
	{
	public:
		Token(Token::TokenType type, const std::string &value)
			:type_(type), value_(value) {
			;
		}

	public:
		inline TokenType type() { return type_; }
		inline std::string	 value() { return value_; }

	private:
		TokenType type_;
		std::string value_;
	};
}


