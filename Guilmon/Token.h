#pragma once

#include <string>

namespace Guilmon {
	enum class TokenType
	{
		KEYWORD,
		NUMBER,
		CHAR,
		VARIABLE,
		TAG,
		END,
		REFER,
	};

	class Token
	{
	public:
		Token(Token::TokenType type, const std::string &value)
			:type_(type), value_(value) {
			;
		}

	public:
		inline TokenType type() const { return type_; }
		inline std::string	 value() const { return value_; }
		inline std::string toString() const{
			return std::string("") +
				"{ " + value_ + " }";
		}

	private:
		TokenType type_;
		std::string value_;
	};
}


