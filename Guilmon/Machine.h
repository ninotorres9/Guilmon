

#pragma once

#include <vector>
#include <map>

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
			op_(op), values_(values) {
			;
		}
		std::string op_;
		std::vector<Token> values_;
	};

	class Machine {
	public:
		Machine(const std::vector<Instruction> &instructions) :
			instructions_(instructions) {
			;
		}
		inline void run() {
			for (auto instruction : instructions_)
				execute(instruction);
		}
	private:
		void execute(Instruction& instruction);
	private:
		std::vector<Instruction> instructions_;
		Stack<int> stack_;
		std::map<std::string, int> variableTable_;
	};

}

