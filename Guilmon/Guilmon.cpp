// Guilmon.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Machine.h"
#include "Parser.h"

using namespace Guilmon;

union demo {
	int number;
	char word;
};

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

//	/*	
//		int i = 0;
//		i = 1;
//	*/
//	std::string text = R"(
//		push 0
//		store %i
//		push 1
//		assign %i
//)";
//

//	/*
//		2 > 1				-> 1
//		2 < 3			-> 1
//		10 == 10		-> 1
//		10 != 10			-> 0
//	*/
//
//	std::string text = R"(
//		push 2
//		push 1
//		gtn
//		push 2
//		push 3
//		ltn
//		push 10
//		push 10
//		eq
//		push 10
//		push 10
//		neq
//)";

//	/*
//		int i = 5;
//		i = i + 1;
//		i -> 6
//	*/
//	std::string text = R"(
//		push 5
//		store %i
//		push %i
//		push  1
//		add
//		assign %i
//)";

	///*
	//	int i = 5;
	//	while(i < 10){
	//	i = i + 1;
	//	}
	//	i -> 10
	//*/
	//std::string text = R"(
	//		push 5
	//		store %i
	//		push %i
	//		push 10
	//		ltn 
	//		jz 11
	//		push %i
	//		push  1
	//		add
	//		assign %i
	//		jmp 2
	//)";



	/* 
		第一趟扫描实验
		将所有tag扔进map
	*/
	

//	std::string text = R"(
//		tag @add
//			add
//			ret
//		tag @main
//			push 2
//			push 1
//			call @add
//			print
//)";
//
//	Parser parser(text);
//	Machine machine(parser.getInstructions());
//	machine.run();

	
	////  allocator 实验
	//std::allocator<int> alloc;
	//auto intPtr = alloc.allocate(1);
	//alloc.construct(intPtr, 5);
	//
	//std::cout << *intPtr  << std::endl;

	//alloc.deallocate(intPtr, 1);

	demo d;
	d.number = 10;
	

	system("pause");
    return 0;
}


