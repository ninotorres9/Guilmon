#include "pch.h"
#include "Parser.h"

using namespace Guilmon;

TEST(ScannerTest, Scanner) {
	Scanner scanner("push 10");
	EXPECT_FALSE(scanner.isEndOfTokens());
	EXPECT_EQ(scanner.peek().value(), "push");
	EXPECT_EQ(scanner.get().type(), TokenType::KEYWORD);
	EXPECT_EQ(scanner.get().type(), TokenType::NUMBER);
	EXPECT_EQ(scanner.get().type(), TokenType::END);
	EXPECT_EQ(scanner.peek().type(), TokenType::END);
	EXPECT_TRUE(scanner.isEndOfTokens());
}

TEST(ScannerTest, Skip) {
	Scanner scanner(R"(
		tag @function
		tag @main
)");
	EXPECT_EQ(scanner.get().value(), "tag");
	EXPECT_EQ(scanner.get().value(), "function");
	EXPECT_EQ(scanner.get().value(), "tag");
	EXPECT_EQ(scanner.get().value(), "main");
	EXPECT_EQ(scanner.get().type(), TokenType::END);
}

TEST(ScannerTest, Store) {
	Scanner scanner("%i");
	auto token = scanner.get();
	EXPECT_EQ(token.type(), TokenType::VARIABLE);
	EXPECT_EQ(token.value(), "i");
}

TEST(ParserTest, Push) {
	Parser parser("push 10");
	auto instructions = parser.getInstructions();
	EXPECT_EQ(instructions[0].op_, "push");
	EXPECT_EQ(instructions[0].operands_[0].type(), TokenType::NUMBER);
}

TEST(ParserTest, Store) {
	Parser parser(R"(
		push 10
		store %i
)");
	auto instructions = parser.getInstructions();
	EXPECT_EQ(instructions[0].op_, "push");
	EXPECT_EQ(instructions[1].op_, "store");
	EXPECT_EQ(instructions[1].operands_[0].type(), TokenType::VARIABLE);
}

TEST(ParserTest, Tag) {
	Parser parser(R"(
		tag @function0
		tag @main
)");
	auto instructions = parser.getInstructions();
	EXPECT_EQ(instructions[0].op_, "tag");
	EXPECT_EQ(instructions[0].operands_[0].type(), TokenType::TAG);
	EXPECT_EQ(instructions[0].operands_[0].value(), "function0");
}

TEST(ParserTest, Demo) {
	Parser parser(R"(
		tag @Person
			push 0
			assign %id
			end_class
		tag @main
			create_class Person person
			push %person.id
			print
)");
	auto instructions = parser.getInstructions();
	int i = 0;
	
}

TEST(ParserTest, Class) {
	Parser parser(R"(
		create_class Demo demo
		assign %Demo.demo
		push %Demo.demo
		class %Demo
)");
	auto instructions = parser.getInstructions();
	EXPECT_EQ(instructions[0].op_, "create_class");
	EXPECT_EQ(instructions[0].operands_[0].type(), TokenType::KEYWORD);
	EXPECT_EQ(instructions[0].operands_[0].value(), "Demo");
	EXPECT_EQ(instructions[0].operands_[1].type(), TokenType::KEYWORD);
	EXPECT_EQ(instructions[0].operands_[1].value(), "demo");

	EXPECT_EQ(instructions[1].operands_[0].type(), TokenType::VARIABLE);
	EXPECT_EQ(instructions[1].operands_[0].value(), "Demo.demo");
	EXPECT_EQ(instructions[2].operands_[0].type(), TokenType::VARIABLE);
	EXPECT_EQ(instructions[2].operands_[0].value(), "Demo.demo");

	EXPECT_EQ(instructions[3].op_, "class");
	EXPECT_EQ(instructions[3].operands_[0].type(), TokenType::VARIABLE);

}