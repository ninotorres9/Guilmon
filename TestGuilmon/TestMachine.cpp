#include "pch.h"
#include "Parser.h"

#include <sstream>
#include <memory>
#include <cctype>


std::string removeSpaces(std::string text) {
	// É¾³ý×Ö·ûÖÐµÄ¿Õ°×·û
	text.erase(std::remove_if(text.begin(), text.end(),
		[](unsigned char x) {return std::isspace(x); }),
		text.end());
	return text;
}

using namespace Guilmon;
class MachineTest : public ::testing::Test {
protected:
	std::stringstream stream_;
	std::streambuf* coutBuf_;
	virtual void SetUp() {
		// ±£´æcout Ö¸Õë
		coutBuf_ = std::cout.rdbuf();
		// ´´½¨strÖ¸Õë
		std::streambuf* strBuf = stream_.rdbuf();
		// Ìæ»»
		std::cout.rdbuf(strBuf);
	}
};

TEST_F(MachineTest, TestAdd) {
	std::string text = R"(
		push 2
		push 1
		add
		print
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "3");
}

TEST_F(MachineTest, TestMultiExpression) {
	// 10 + 2 * 5 / 2 - 9 = 6
	std::string text = R"(
			push 10
			push 2
			push 5
			mul
			push 2
			div
			add
			push 9
			sub
			print
	)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "6");
}

TEST_F(MachineTest, TestGreaterThan) {
	// 2 > 1
	std::string text = R"(
		push 2
		push 1
		gtn
		print
	)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "1");
}

TEST_F(MachineTest, TestGreaterThanOrEqual) {
	// 2 > 1
	std::string text = R"(
		push 2
		push 1
		goe
		print
	)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "1");
}

TEST_F(MachineTest, TestAnd) {
	// 2 > 3 && 3 < 5
	std::string text = R"(
		push 2
		push 3
		gtn
		push 3
		push 5
		ltn
		and
		print
	)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "0");
}

TEST_F(MachineTest, TestOr) {
	// 3 > 2 || 3 < 1
	std::string text = R"(
		push 3
		push 2
		gtn
		push 3
		push 1
		ltn
		or
		print
	)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "1");
}




