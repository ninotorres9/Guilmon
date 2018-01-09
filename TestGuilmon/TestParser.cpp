#include "pch.h"
#include "Parser.h"

using namespace Guilmon;

TEST(TestParser, Scanner) {
	Scanner scanner("push 10");
	EXPECT_FALSE(scanner.isEndOfTokens());
	EXPECT_EQ(scanner.peek().value(), "push");
	EXPECT_EQ(scanner.get().type(), TokenType::KEYWORD);
	EXPECT_EQ(scanner.get().type(), TokenType::NUMBER);
	EXPECT_EQ(scanner.get().type(), TokenType::END);
	EXPECT_EQ(scanner.peek().type(), TokenType::END);
	EXPECT_TRUE(scanner.isEndOfTokens());
}

TEST(TestParser, Push) {
	Parser parser("push 10");
	auto instructions = parser.getInstructions();
	EXPECT_EQ(instructions[0].op_, "push");
}

TEST(TestParser, Store) {
	Parser parser(R"(
		push 10
		store %i
)");
	auto instructions = parser.getInstructions();
	EXPECT_EQ(instructions[0].op_, "push");
	EXPECT_EQ(instructions[1].op_, "store");
	// EXPECT_EQ(instructions[1].values_[0], "")
}