// Guilmon.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Machine.h"
#include "Parser.h"

using namespace Guilmon;

int main()
{

	//// 10 + 2 * 5 / 2 - 9 = 21
	//std::string text = R"(
	//		push 10
	//		push 2
	//		push 5
	//		mul
	//		push 2
	//		div
	//		add
	//		push 9
	//		sub
	//)";

//	// int i = 10;
//	std::string text = R"(
//		push 10
//		store %i
//)";

//	// int i = 10; 20 + i;
//	std::string text = R"(
//		push 10
//		store %i
//		push 20
//		push %i
//		add
//)";

//	// jmp
//	std::string text = R"(
//		jmp 2
//		push 10
//		push 20
//		push 12
//		add
//)";

//	//jz jump if zero 针对栈顶元素，如果栈顶元素非零，不跳转，指令+1
//	// 否则跳转到指定位置
//	std::string  text = R"(
//		push 1
//		jz 3
//		push 10
//		push 12
//		push 20
//		add
//)";

	/*	
		int i = 0;
		i = 1;
	*/
	std::string text = R"(
		push 0
		store %i
		push 1
		assign %i
)";

	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	system("pause");
    return 0;
}

