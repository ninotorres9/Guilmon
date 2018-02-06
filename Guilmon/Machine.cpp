#include "stdafx.h"
#include "Machine.h"

namespace Guilmon {
	void Machine::execute() {
		auto instruction = instructions_[index_++];
		auto op = instruction.op_;
		if (op == "push") {
			// value两种情况: 1. 变量调用 2.字面值
			if (instruction.operands_[0].type() == TokenType::VARIABLE) {
				auto valuePtr = getVariable(instruction.operands_[0].value());
				stack_.push(*valuePtr);
			}
			else {
				auto value = std::stoi(instruction.operands_[0].value());
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
		else if (op == "gtn") {
			int rhs = stack_.pop();
			int lhs = stack_.pop();
			stack_.push(lhs > rhs ? 1 : 0);
		}
		else if (op == "goe") {
			// greater than or equal
			int rhs = stack_.pop();
			int lhs = stack_.pop();
			stack_.push(lhs >= rhs ? 1 : 0);
		}
		else if (op == "ltn") {
			int rhs = stack_.pop();
			int lhs = stack_.pop();
			stack_.push(lhs < rhs ? 1 : 0);
		}
		else if (op == "loe") {
			// less than or equal
			int rhs = stack_.pop();
			int lhs = stack_.pop();
			stack_.push(lhs <= rhs ? 1 : 0);
		}
		else if (op == "eq") {
			int rhs = stack_.pop();
			int lhs = stack_.pop();
			stack_.push(lhs == rhs ? 1 : 0);
		}
		else if (op == "neq") {
			int rhs = stack_.pop();
			int lhs = stack_.pop();
			stack_.push(lhs != rhs ? 1 : 0);
		}
		else if (op == "and") {
			int rhs = stack_.pop();
			int lhs = stack_.pop();
			stack_.push( (lhs == rhs == 1) ? 1 : 0);
		}
		else if (op == "or") {
			int rhs = stack_.pop();
			int lhs = stack_.pop();
			stack_.push((lhs ==1 || rhs == 1) ? 1 : 0);
		}
		else if (op == "store") {
			auto name = instruction.operands_[0].value();
			auto valuePtr = createInt(stack_.pop());
			intVarTable_.insert({ name, valuePtr });
		}
		else if (op == "assign") {
			auto name = instruction.operands_[0].value();
			auto valuePtr = createInt(stack_.pop());
			intVarTable_.find(name)->second = valuePtr;
		}
		else if (op == "jmp") {
			// 直接跳转
			size_t offset;
			// 向tag跳转
			if (instruction.operands_[0].type() == TokenType::TAG) {
				offset = functionTable_.find(instruction.operands_[0].value())->second;
			}
			// 跳转至固定位置
			else {
				offset = std::stoi(instruction.operands_[0].value());
			}
			index_ = offset;
		}
		else if (op == "jz") {
			// stack顶端值为0时跳转
			auto isZero = (stack_.pop() == 0 ? true : false);
			if (isZero) {
				size_t offset;
				if (instruction.operands_[0].type() == TokenType::TAG) {
					offset = functionTable_.find(instruction.operands_[0].value())->second;
				}
				else {
					offset = std::stoi(instruction.operands_[0].value());
				}
				index_ = offset;
			}
		}
		else if (op == "jnz") {
			// stack顶端值为 非0 时跳转
			auto isNotZero = (stack_.pop() != 0 ? true : false);
			if (isNotZero) {
				auto offset = std::stoi(instruction.operands_[0].value());
				index_ = offset;
			}
		}
		else if (op == "print") {
			auto value = stack_.pop();
			std::cout << value << std::endl;
		}
		else if (op == "call") {
			// 存入当前地址并跳转
			addressStack_.push(index_);
			size_t offset = functionTable_.find(instruction.operands_[0].value())->second;
			index_ = offset;
		}
		else if (op == "ret") {
			// 跳回调用地址
			size_t offset = addressStack_.pop();
			index_ = offset;
		}
		else {
			;
		}
	}
}

