

#pragma once

#include <vector>
#include <map>
#include <iostream>

#include "Token.h"

namespace Guilmon {
	template<typename T>
	class Stack {
	public:
		inline void push(T value) {
			container_.push_back(value);
		}
		inline T pop() {
			auto result = container_.back();
			container_.pop_back();
			return result;
		}
	private:
		std::vector<T> container_;
	};

	class Instruction {
	public:
		Instruction(const std::string& op, const std::vector<Token> &values) :
			op_(op), operands_(values) {
			;
		}
		std::string op_;
		std::vector<Token> operands_;
	};


	class Machine {
	public:
		Machine(const std::vector<Instruction> &instructions) :
			instructions_(instructions) , index_(0){
			;
		}
		inline void run() {

			// 第一趟扫描
			size_t index = 0;
			for (auto inst : instructions_) {
				if (inst.op_ == "tag")
					functionTable_.insert({ inst.operands_[0].value(), index});
				index++;
			}

			// 从main处启动
			auto main = functionTable_.find("main");
			if (main != functionTable_.end()) {
				index_ = main->second;

				// 这里不用for each是为了方便jmp等命令跳转
				while (index_ < instructions_.size()) {
					execute();
				}
			}
			else {
				// 报错，没有main
			}


		}
	private:
		void execute();
		inline int* createInt(int value) {
			auto ptr = intAlloc_.allocate(1);
			intAlloc_.construct(ptr, value);
			return ptr;
		}
		inline int* getVariable(const std::string& name)
		{
			return intVarTable_.find(name)->second;
		}
	private:
		std::vector<Instruction> instructions_;
		size_t index_;
		Stack<int> stack_;
		Stack<size_t> addressStack_;
		std::map<std::string, int*> intVarTable_;
		std::allocator<int> intAlloc_;
		std::map<std::string, size_t> functionTable_;
	};

}

