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

	// int i = 10;
	std::string text = R"(
		push 10
		store %i
)";
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	system("pause");
    return 0;
}

