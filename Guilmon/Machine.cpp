#include "stdafx.h"
#include "Machine.h"

namespace Guilmon {
	void Machine::execute() {
		auto instruction = instructions_[index_++];
		auto op = instruction.op_;
		if (op == "push") {
			// 变量
			if (instruction.operands_[0].type() == TokenType::VARIABLE) {
				auto name = instruction.operands_[0].value();

				if (state_ == State::INDEX) {
					// 数组
					auto arrayPtr = findVariable(name);
					auto offset = operationStack_.pop().number;
					operationStack_.push(*(arrayPtr + offset));
					state_ = State::VALUE;
				}
				else {
					// 普通变量
					auto variablePtr = findVariable(name);
					operationStack_.push(Value{ *variablePtr });
				}
			}
			// 字符
			else if (instruction.operands_[0].type() == TokenType::CHAR) {
				auto value = instruction.operands_[0].value()[0];
				operationStack_.push(Value{ value });
			}
			// 数字
			else {
				auto value = std::stoi(instruction.operands_[0].value());
				operationStack_.push(Value{ value });
			}
		}
		else if (op == "add") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			operationStack_.push(Value{ lhs + rhs });
		}
		else if (op == "sub") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			operationStack_.push(Value{ lhs - rhs });
		}
		else if (op == "mul") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			operationStack_.push(Value{ lhs * rhs });
		}
		else if (op == "div") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			operationStack_.push(Value{ lhs / rhs });
		}
		else if (op == "gtn") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			operationStack_.push(Value{ lhs > rhs ? 1 : 0 });
		}
		else if (op == "goe") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			operationStack_.push(Value{ lhs >= rhs ? 1 : 0 });
		}
		else if (op == "ltn") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			operationStack_.push(Value{ lhs < rhs ? 1 : 0 });
		}
		else if (op == "loe") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			operationStack_.push(Value{ lhs <= rhs ? 1 : 0 });
		}
		else if (op == "eq") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			operationStack_.push(Value{ lhs == rhs ? 1 : 0 });
		}
		else if (op == "neq") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			operationStack_.push(Value{ lhs != rhs ? 1 : 0 });
		}
		else if (op == "and") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			operationStack_.push(Value{ (lhs == rhs == 1) ? 1 : 0 });
		}
		else if (op == "or") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			operationStack_.push(Value{ (lhs == 1 || rhs == 1) ? 1 : 0 });
		}
		else if (op == "free") {
			auto name = instruction.operands_[0].value();
			auto value = findVariable(name);
			alloc_.deallocate(value, sizeof(Value));
			deleteVariable(name);
		}
		else if (op == "index") {
			state_ = State::INDEX;
		}
		else if (op == "array") {
			state_ = State::ARRAY;
		}
		else if (op == "assign") {

			auto name = instruction.operands_[0].value();
			if(state_ == State::ARRAY){
				// 创建数组
				auto size = operationStack_.pop().number;
				auto valuePtr = createArray(size);
				setArray(name, valuePtr, size);
				state_ = State::VALUE;
			}
			else if (state_ == State::INDEX){ // isIndex_ == true) {
				// 更改数组
				auto offset = operationStack_.pop().number;	// index
				auto valuePtr = createValue(operationStack_.pop().number);
				*(findVariable(name) + offset) = *valuePtr;
				// isIndex_ = false;
				state_ == State::VALUE;
			}
			else {
				// 普通变量
				auto valuePtr = createValue(operationStack_.pop().number);
				setVariable(name, valuePtr);
			 }
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
			auto isZero = (operationStack_.pop().number == 0 ? true : false);
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
			auto isNotZero = (operationStack_.pop().number != 0 ? true : false);
			if (isNotZero) {
				auto offset = std::stoi(instruction.operands_[0].value());
				index_ = offset;
			}
		}
		else if (op == "print") {
			auto value = operationStack_.pop();
			std::cout << value.number << std::endl;
		}
		else if (op == "print_s") {
			auto value = operationStack_.pop();
			std::cout << value.word << std::endl;
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

