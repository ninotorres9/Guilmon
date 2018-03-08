#include "stdafx.h"
#include "Machine.h"

namespace Guilmon {
	void Machine::execute() {
		auto instruction = instructions_[index_++];
		auto op = instruction.op_;
		if (op == "push") {
			// value几种情况: 1. 变量 2.char 3.数字
			if (instruction.operands_[0].type() == TokenType::VARIABLE) {
				auto valuePtr = findVariable(instruction.operands_[0].value());
				operationStack_.push(Value{ *valuePtr });
			}
			else if (instruction.operands_[0].type() == TokenType::CHAR) {
				// 此处string转char，为了方便初始化union(蛋疼。。。）
				auto value = instruction.operands_[0].value()[0];
				operationStack_.push(Value{ value });
			}
			else {
				auto value = std::stoi(instruction.operands_[0].value());
				operationStack_.push(Value{ value });
			}
		}
		else if (op == "push_a") {
			// 压入数组元素
			auto name = instruction.operands_[0].value();	// 变量名
			auto arrayPtr = findVariable(name);
			auto offset = operationStack_.pop().number;
			operationStack_.push(*(arrayPtr + offset));
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
			// greater than or equal
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
			// less than or equal
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
		else if (op == "store") {
			auto name = instruction.operands_[0].value();	// 变量名
			auto valuePtr = createValue(operationStack_.pop().number);
			variableTable_.createVariable(name, valuePtr);
		}
		else if (op == "free") {
			auto name = instruction.operands_[0].value();
			auto value = variableTable_.findVariable(name);
			alloc_.deallocate(value, sizeof(Value));
			variableTable_.deleteVariable(name);
		}
		else if (op == "assign_a") {
			auto name = instruction.operands_[0].value();	// 变量名
			auto arrayPtr = findVariable(name);
			auto offset = operationStack_.pop().number;
			auto value = operationStack_.pop();
			alloc_.construct(arrayPtr + offset, value);
		}
		else if (op == "store_a") {
			auto name = instruction.operands_[0].value();	// 变量名
			auto size = operationStack_.pop().number;
			auto value = createArray(size);
			createVariable(name, value);
		}
		//else if (op == "store_a") {
		//	auto name = instruction.operands_[0].value();	// 变量名
		//	auto arrayPtr = findVariable(name);
		//	auto offset = operationStack_.pop().number;
		//	auto value = operationStack_.pop();
		//	alloc_.construct(arrayPtr + offset, value);
		//}
		//else if (op == "new_array") {
		//	auto name = instruction.operands_[0].value();	// 变量名
		//	auto size = operationStack_.pop().number;
		//	auto value = createArray(size);
		//	createVariable(name, value);
		//}
		else if (op == "assign") {
			auto name = instruction.operands_[0].value();
			auto value = createValue(operationStack_.pop().number);
			setVariable(name, value);
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
		else if (op == "newscope") {
			variableTable_.addSubScope();
		}
		else if (op == "exitscope") {
			variableTable_.exitScope();
		}
		else {
			;
		}
	}
}

