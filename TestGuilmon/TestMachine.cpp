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

TEST_F(MachineTest, TestNoTab) {
	std::string text = R"(tag main
push 123
print
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "123");
}

TEST_F(MachineTest, TestAdd) {
	std::string text = R"(
		tag main
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
	tag main
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
	tag main
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
	tag main
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
	tag main
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
	tag main
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

TEST_F(MachineTest, TestAssign) {
	/*
		int number = 5;
		number = number + 10;
		-> 15
	*/
	std::string text = R"(
	tag @main
		push 5
		store %number
		push %number
		push 10
		add
		assign %number
		push %number
		print 
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "15");
}

TEST_F(MachineTest, TestChar) {
	/*
	char ch = 'a'
	print 'a'
	-> 15
	*/
	std::string text = R"(
	tag @main
		push 'd'
		store %ch
		push %ch
		print_s 
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "d");
}

TEST_F(MachineTest, TestIf) {
	/*
		int cond = 1;
		if(cond > 0)
			print 5;
		else
			print 7;
	*/
	std::string text = R"(
	tag @main
		push 1
		store %cond
		push %cond
		push 0
		gtn
		jz @ELSE
	tag @IF
		push 5
		print
		jmp @ENDIF
	tag @ELSE
		push 7
		print
	tag @ENDIF
	)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "5");
}

TEST_F(MachineTest, TestScope) {
	/*
		
	*/
}

TEST_F(MachineTest, TestCallFunciton) {
	/*
		int add(int lhs, int rhs){
			return lhs + rhs;
		}
		void main(){
			print add(5, 6);
		}
	*/
	std::string text = R"(
	tag @add
		add
		ret
	tag @main
		push 5
		push 6
		call @add
		print
	)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "11");
}

TEST_F(MachineTest, TestWhile) {
	/*
	void main(){
		int condition = 10;
		while(condition > 5){
			condition = condition - 1;
		}
		print condition;
	}
	-> 5


	*/
	std::string text = R"(
	tag @main
		push 10
		store %condition
	tag @WHILE
		push %condition
		push 5
		gtn
		jz @ENDWHILE
		push %condition
		push 1
		sub
		assign %condition
		jmp @WHILE
	tag @ENDWHILE	
	push %condition
	print
	)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "5");
}

TEST_F(MachineTest, TestFor) {
	/*
		int result = 0;
		for(int i = 0; i != 10; i = i + 1){
			result = result + 1;
		}
		print result;
		
		-> 10
	*/
	std::string text = R"(
	tag @main
		push 0
		store %result
	tag @FOR
		push 0
		store %i
	tag @LOOP
		push %i
		push 10
		neq
		jz @ENDFOR
		push %result
		push 1
		add
		assign %result
		push %i
		push 1
		add
		assign %i
		jmp @LOOP
	tag @ENDFOR
		push %result
		print %result
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "10");
}

TEST_F(MachineTest, TestArray) {
	/*
		int array[3] = {19, 29, 13};
		print array[0]

		-> 3
	*/
	std::string text = R"(
	tag @main
		push 3
		new_array %array
		push 19
		push 0
		store_a %array
		push 0
		push_a %array 
		print
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "19");
}




