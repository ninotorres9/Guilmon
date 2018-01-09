#include "stdafx.h"
#include "Machine.h"

namespace Guilmon {
	void Machine::execute() {
		auto instruction = instructions_[index_++];
		auto op = instruction.op_;
		if (op == "push") {
			// value两种情况: 1. 变量调用 2.字面值
			if (instruction.values_[0].type() == TokenType::VARIABLE) {
				auto value = variableTable_.find(instruction.values_[0].value())->second;
				stack_.push(value);
			}
			else {
				auto value = std::stoi(instruction.values_[0].value());
				stack_.push(value);
			}
		}
		else if (op == "add") {
			int rhs = stack_.pop();
			int lhs = stack_.pop();
			stack_.push(lhs + rhs);
		}
		else if (op == "sub") {
			int rhs = stack_.pop();
			int lhs = stack_.pop();
			stack_.push(lhs - rhs);
		}
		else if (op == "mul") {
			int rhs = stack_.pop();
			int lhs = stack_.pop();
			stack_.push(lhs * rhs);
		}
		else if (op == "div") {
			int rhs = stack_.pop();
			int lhs = stack_.pop();
			stack_.push(lhs / rhs);
		}
		else if (op == "store") {
			auto name = instruction.values_[0].value();
			auto value = stack_.pop();
			variableTable_.insert({ name, value });
		}
		else if (op == "jmp") {
			auto offset = std::stoi(instruction.values_[0].value());
			index_ = offset;
		}
		else {
			;
		}
	}
}

