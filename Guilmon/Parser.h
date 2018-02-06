#pragma once

#include "Machine.h"
#include "Token.h"

#include <string>

namespace Guilmon {

	class Scanner
	{
	public:
		Scanner(std::string exp) :exp_(exp), index_(0) {
			getTokens();	// generate tokens
			iter_ = tokens_.begin();
		}

	public:
		inline Token get() {
			return isEndOfTokens() ? Token(TokenType::END, "end") : *(iter_++);
		}
		inline Token peek() {
			return isEndOfTokens() ? Token(TokenType::END, "end") : *(iter_);
		}
		inline bool isEndOfTokens() {
			return iter_ >= tokens_.end() ? true : false;
		}

	private:
		inline std::vector<Token> getTokens() {
			while (!isEndOfExp())
				tokens_.push_back(getToken());
			return tokens_;
		}
		Token getToken();
		inline char peekChar() {
			return isEndOfExp() ? EOF : exp_[index_];
		}
		inline char getChar() {
			return isEndOfExp() ? EOF : exp_[index_++];
		}
		inline void skipSpace() {
			// Ìø¹ýÈßÓà×Ö·û
			while(isspace(peekChar()))
				getChar();
		}
		inline bool isEndOfExp() {
			return index_ >= exp_.size() ? true : false;
		}
		inline Token getNumberToken() {
			std::string value;
			value.append(getInteger());
			if (peekChar() == '.')
				value.append(getDecimal());
			return Token(TokenType::NUMBER, value);
		}
		inline std::string getInteger() {
			std::string value;
			while (isdigit(peekChar()) && !isEndOfExp())
				value.push_back(getChar());
			return value;
		}
		inline std::string getDecimal() {
			std::string value;
			value.push_back(getChar());		// .
			value.append(getInteger());
			return value;
		}
		inline Token getVariableToken() {
			getChar();	// eat %
			return Token(TokenType::VARIABLE, getIdentifier());
		}
		inline Token getKeywordToken() {
			return Token(TokenType::KEYWORD, getIdentifier());
		}
		inline std::string getIdentifier() {
			std::string value;
			while (isalpha(peekChar()) || peekChar() == '_')
				value.push_back(getChar());
			return value;
		}

	private:
		std::string	exp_;
		size_t index_;
		std::vector<Token> tokens_;
		std::vector<Token>::iterator iter_;
	};

	class Parser {
	public:
		Parser(const std::string& text) :
			scanner_(text) {
			;
		}
		inline std::vector<Instruction> getInstructions() {
			std::vector<Instruction> instructions;
			while (!scanner_.isEndOfTokens())
				instructions.push_back(expression());
			return instructions;
		}

	private:
		Instruction expression();
	private:
		Scanner scanner_;
	};


}