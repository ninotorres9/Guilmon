#include "pch.h"
#include "Parser.h"

#include <sstream>
#include <memory>
#include <cctype>


std::string removeSpaces(std::string text) {
	// 删除字符中的空白符
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
		// 保存cout 指针
		coutBuf_ = std::cout.rdbuf();
		// 创建str指针
		std::streambuf* strBuf = stream_.rdbuf();
		// 替换
		std::cout.rdbuf(strBuf);
	}
};

TEST_F(MachineTest, TestNoTab) {
	std::string text = R"(
		tag main
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
		number = 5;
		number = number + 10;
		print number
		-> 15
	*/
	std::string text = R"(
	tag @main
		push 5
		assign %number
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
		ch = 'a'
		print 'a'
		-> 15
	*/
	std::string text = R"(
		tag @main
			push 'd'
			assign %ch
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
		def main(){
			if(2>1){
				print 1;
			}else{
				print 2;
			}
		}
	*/
	std::string text = R"(
		tag @main
        tag @IF_CONDITION_0
            push 2
            push 1
            gtn
            jz @ELSE_BODY_0
        tag @IF_BODY_0
            push 1
            print
            jmp @END_IF_0
        tag @ELSE_BODY_0
            push 2
            print
        tag @END_IF_0
	)";

	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "1");
}

TEST_F(MachineTest, TestMultIf) {
	/*
		def main(){
			if(1>5){
				print 1;
			}else if(3<1){
				print 2;
			}else{
				print 3;
			}
		}

	*/
	std::string text = R"(
	    tag @main
        tag @IF_CONDITION_1
            push 1
            push 5
            gtn
            jz @ELSE_BODY_1
        tag @IF_BODY_1
            push 1
            print
            jmp @END_IF_1
        tag @ELSE_BODY_1
        tag @IF_CONDITION_0
            push 3
            push 1
            ltn
            jz @ELSE_BODY_0
        tag @IF_BODY_0
            push 2
            print
            jmp @END_IF_0
        tag @ELSE_BODY_0
            push 3
            print
        tag @END_IF_0
        tag @END_IF_1
	)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "3");
}

TEST_F(MachineTest, TestWhile) {
	/*
		def main(){
			len = 10;
			while(len > 0){
				len -= 1;
			}
			print len;
		}

		-> 0
	*/
	std::string text = R"(
	tag @main
		push 10
		assign %len
	tag @WHILE_CONDITION_0
		push %len
		push 0
		gtn
		jz @END_WHILE_0
	tag @WHILE_BODY_0
		push %len
		push 1
		sub
		assign %len
		jmp @WHILE_CONDITION_0
	tag @END_WHILE_0
	push %len
	print
	)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "0");
}

TEST_F(MachineTest, TestFor) {
	/*
		def main(){
			result = 0;
			for(i = 0; i != 10; i+=1){
				result += i;
			}
			print result;
		}	
	*/
	std::string text = R"(
		tag @main
			push 0
			assign %result
		tag @FOR_INITIALIZATION_0
			push 0
			assign %i
		tag @FOR_CONDITION_0
			push %i
			push 10
			neq
			jz @END_FOR_0
		tag @FOR_BODY_0
			push %result
			push %i
			add
			assign %result
		tag @FOR_INCREMENT_0
			push %i
			push 1
			add
			assign %i
			jmp @FOR_CONDITION_0
		tag @END_FOR_0
			push %result
			print
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "45");
}

TEST_F(MachineTest, TestArray) {
	/*
		array = [5, 2, 3];
		print array[1];

		-> 3
	*/
	std::string text = R"(
	tag @main
		push 3
		push 2
		push 5
		push 3
		array
		assign %array
		push 1
		index
		push %array
		print

)";
	/*
	push 0
	push &
	push %array
	print
	*/
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "2");
}

TEST_F(MachineTest, TestArray_1) {
	std::string text = R"(
		tag @main
		  push 6
		  push 5
		  push 4
		  push 3
		  push 2
		  push 1
		  push 6
		  array
		  assign %container
		tag @FOR_INITIALIZATION_0
		  push 0
		  assign %i
		tag @FOR_CONDITION_0
		  push %i
		  push 6
		  neq
		  jz @END_FOR_0
		tag @FOR_BODY_0
		  push %i
		  index
		  push %container
		  print
		tag @FOR_INCREMENT_0
		  push %i
		  push 1
		  add
		  assign %i
		  jmp @FOR_CONDITION_0
		tag @END_FOR_0
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "123456");
}

TEST_F(MachineTest, TestArray_2) {
	/*
		def main(){
			container = [1, 2, 3];
			print container[0];
			container[0] = 5;
			print container[0];
		}
	*/
	std::string text = R"(
		tag @main
			push 3
			push 2
			push 1
			push 3
			array
			assign %container
			push 9
			push 0
			index
			assign %container
			push 0
			index
			push %container
			print
			push 2
			index
			push %container
			print
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "93");

}

TEST_F(MachineTest, TestArray_3) {
	/*
	*/
	std::string text = R"(
		tag @main
		  push 6
		  push 5
		  push 4
		  push 3
		  push 2
		  push 1
		  push 6
		  array
		  assign %source
		tag @FOR_INITIALIZATION_0
		  push 0
		  assign %i
		tag @FOR_CONDITION_0
		  push %i
		  push 6
		  neq
		  jz @END_FOR_0
		tag @FOR_BODY_0
		  push 1
		  print
		tag @FOR_INCREMENT_0
		  push %i
		  push 1
		  add
		  assign %i
		  jmp @FOR_CONDITION_0
		tag @END_FOR_0
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "111111");

}

TEST_F(MachineTest, TestArray_4) {
	/*
		def main(){
			source = [1, 2, 3, 4, 5, 6];
			for(i = 0; i != 6; i += 1){
				source[i] = 2;
				print source[i];
			}
		}
	*/
	std::string text = R"(
		tag @main
			push 6
			push 5
			push 4
			push 3
			push 2
			push 1
			push 6
			array
			assign %source
		tag @FOR_INITIALIZATION_0
			push 0
			assign %i
		tag @FOR_CONDITION_0
			push %i
			push 6
			neq
			jz @END_FOR_0
		tag @FOR_BODY_0
			push 2
			push %i
			index
			assign %source
			push %i
			index
			push %source
			print
		tag @FOR_INCREMENT_0
			push %i
			push 1
			add
			assign %i
			jmp @FOR_CONDITION_0
		tag @END_FOR_0
)";

	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "222222");
}

TEST_F(MachineTest, TestCallFunciton) {
	/*
	def func(){
		return 5;
	}

	def main(){
		print func();
	}
	*/
	std::string text = R"(
	tag @func
		push 59
		ret
	tag @main
		call @func
		print
	)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "59");
}

TEST_F(MachineTest, TestCallFunciton_1) {
	/*
		def add(lhs, rhs){
			return lhs + rhs;
		}

		def main(){
			print add(1, 2);
		}
	*/

	std::string text = R"(
		tag @add
			assign %lhs
			assign %rhs
			push %lhs
			push %rhs
			add
			ret
		tag @main
			push 2
			push 1
			call @add
			print
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "3");
}

TEST_F(MachineTest, TestNotDefinedVar) {
	/*
		def main(){
			value = 123;
			print value;
			print abc;
		}
	*/
	std::string text = R"(
		tag @main
			push 123
			assign %value
			push %value
			print
			push %akb
			print
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	EXPECT_EQ(removeSpaces(stream_.str()), "123name'akb'isnotdefined");
}

TEST_F(MachineTest, TestClass) {
	/*
		class Demo{
		public:
			int id;
		}

		int main(){
			Demo demo();
			demo.id = 10;
			print demo.id
		}
		-> 10
	*/

	/*
		建立一个function table
		加到哪里？
		加到Scope里？
		不对，应该做一个全局的functionTable
		先去git一下做个备份
	*/
}




