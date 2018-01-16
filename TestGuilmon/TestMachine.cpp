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
	EXPECT_EQ(removeSpaces(stream_.str()), "3");
}