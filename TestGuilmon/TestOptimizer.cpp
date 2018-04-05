#include "pch.h"
#include "Optimizer.h"

using namespace Guilmon;

TEST(ToolTest, TestIsDigit) {
	std::string digit = "132";
	std::string text = "a123";
	EXPECT_TRUE(isDigit(digit));
	EXPECT_FALSE(isDigit(text));
}

TEST(ToolTest, TestIsConstant) {
	Instruction instruction("push", { Token(TokenType::NUMBER, "15") });
	EXPECT_TRUE(isConstant(instruction));
}

TEST(OptimizerTest, TestConstantFolding) {
	std::string source = R"(
	tag @main
		push 1
		push 2
		add
		push 15
		push 5
		sub
)";
	std::string answer = R"(
	tag @main
		push 3
		push 10
		
)";
	auto result = Optimizer(Parser(source).getInstructions()).getInstructions();
	auto expected = Optimizer(Parser(answer).getInstructions()).getInstructions();
	
	ASSERT_EQ(result.size(), expected.size());
	for (int i = 0; i < result.size(); ++i)
	{
		EXPECT_EQ(result[i].op_, expected[i].op_) << "different operator at index: " << i;
		EXPECT_EQ(result[i].operands_[0].toString(), expected[i].operands_[0].toString()) << "different operands at index: " << i;
	}

}
