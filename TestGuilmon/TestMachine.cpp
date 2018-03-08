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

TEST_F(MachineTest, TestMult) {
	std::string text = R"(
		tag main
			push 1
			push 5
			add
			push 6
			add
			push 7
			add
			print
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "19");
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

TEST_F(MachineTest, TestDiv) {
	std::string text = R"(
		tag main
			push 10
			push 2
			div
			print
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "5");
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

TEST_F(MachineTest, TestMultIf) {
	/*
		int main(){
		int result = 20;

		if(1){
			result += 5;
		}else{
			result += 21;
		}

		if(0){
			result += 19;
		}else{
			result += 7;
		}

		print result;
}
	*/
	std::string text = R"(
		tag @main 
			push 20 
			store %result 
			push 1 
			jz @ELSE0 
		tag @IF0 
			push %result 
			push 5 
			add 
			assign %result 
			jmp @ENDIF0 
		tag @ELSE0 
			push %result 
			push 21 
			add 
			assign %result 
		tag @ENDIF0 
			push 0 
			jz @ELSE1 
		tag @IF1 
			push %result 
			push 19 
			add 
			assign %result 
			jmp @ENDIF1 
		tag @ELSE1 
			push %result 
			push 7 
			add 
			assign %result 
		tag @ENDIF1 
			push %result 
			print 

	)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "32");
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
		print
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "10");
}

TEST_F(MachineTest, TestNestedScope) {
	/*
	int i = 50;
	for(int i =0; i != 10; ++i){
		;
	}
	print i;
	*/
	std::string text = R"(
	tag @main
		push 50
		store %i
	tag @FOR
		newscope
		push 0
		store %i
	tag @LOOP
		push %i
		push 10
		neq
		jz @ENDFOR
		push %i
		push 1
		add
		assign %i
		jmp @LOOP
	tag @ENDFOR
		exitscope
		push %i
		print
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "50");
}

TEST_F(MachineTest, TestNestedFor) {
	/*
	int main(){
		int result = 0;
		for(int i = 0; i != 5; i++){
			for(int j = 0; j != 5; j++){
				result++;
			}
		}
		print result;
	}
	-> 25
	*/

	std::string text = R"(
	tag @main 
		push 0 
		store %result 
	tag @FOR0 
		newscope 
		push 0 
		store %i 
	tag @LOOP0 
		push %i 
		push 5 
		neq 
		jz @ENDFOR0 
	tag @FOR1 
		newscope 
		push 0 
		store %j 
	tag @LOOP1 
		push %j 
		push 5 
		neq 
		jz @ENDFOR1 
		push %result 
		push 1 
		add 
		assign %result 
		push %j 
		push 1 
		add 
		assign %j 
		jmp @LOOP1 
	tag @ENDFOR1 
		exitscope 
		push %i 
		push 1 
		add 
		assign %i 
		jmp @LOOP0 
	tag @ENDFOR0 
		exitscope 
		push %result 
		print 
)";

	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "25");
}

TEST_F(MachineTest, TestArray) {
	/*
		int[3] array;
		array[0] = 19;
		print array[0]

		-> 3
	*/
//	std::string text = R"(
//	tag @main
//		push 3
//		new_array %array
//		push 19
//		push 0
//		store_a %array
//		push 0
//		push_a %array 
//		print
//)";
	std::string text = R"(
	tag @main
		push 3
		store_a %array
		push 19
		push 0
		assign_a %array
		push 0
		push_a %array 
		print
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "19");
}




