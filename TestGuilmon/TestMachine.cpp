#include "pch.h"
#include "Parser.h"

#include <sstream>
#include <memory>

using namespace Guilmon;
class MachineTest : public ::testing::Test {
protected:
	std::stringstream stream_;
	std::streambuf* coutBuf_;
	virtual void SetUp() {
		// 保存cout 指针
		coutBuf_ = std::cout.rdbuf();
		// 创建str指针
		std::streambuf* strBuf = stream_.rdbuf();
		// 替换
		std::cout.rdbuf(strBuf);
	}
};

TEST_F(MachineTest, Machine) {
	std::string text = R"(
		push 2
		push 1
		add
		print
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();

	std::string result;
	stream_ >> result;
	EXPECT_EQ(result, "3");
}