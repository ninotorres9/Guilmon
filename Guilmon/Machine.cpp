#include "stdafx.h"
#include "Machine.h"

namespace Guilmon {
	void Machine::execute() {
		auto instruction = instructions_[index_++];
		auto op = instruction.op_;
		if (op == "push") {
			handlePushInstruction(instruction);
		}
		else if (op == "add") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			pushVariable(new Value{ lhs + rhs });
		}
		else if (op == "sub") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			pushVariable(new Value{ lhs - rhs });
		}
		else if (op == "mul") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			pushVariable(new Value{ lhs * rhs });
		}
		else if (op == "div") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			pushVariable(new Value{ lhs / rhs });
		}
		else if (op == "gtn") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			pushVariable(new Value{ lhs > rhs ? 1 : 0 });
		}
		else if (op == "goe") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			pushVariable(new Value{ lhs >= rhs ? 1 : 0 });
		}
		else if (op == "ltn") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			pushVariable(new Value{ lhs < rhs ? 1 : 0 });
		}
		else if (op == "loe") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			pushVariable(new Value{ lhs <= rhs ? 1 : 0 });
		}
		else if (op == "eq") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			pushVariable(new Value{ lhs == rhs ? 1 : 0 });
		}
		else if (op == "neq") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			pushVariable(new Value{ lhs != rhs ? 1 : 0 });
		}
		else if (op == "and") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			pushVariable(new Value{ (lhs == rhs == 1) ? 1 : 0 });
		}
		else if (op == "or") {
			int rhs = operationStack_.pop().number;
			int lhs = operationStack_.pop().number;
			pushVariable(new Value{ (lhs == 1 || rhs == 1) ? 1 : 0 });
		}
		else if (op == "free") {
			auto name = instruction.operands_[0].value();
			auto value = findVariable(name);
			alloc_.deallocate(value, sizeof(Value));
			deleteVariable(name);
		}
		else if (op == "index") {
			setState(State::INDEX);
		}
		else if (op == "array") {
			setState(State::ARRAY);
		}
		else if (op == "assign") {
			handleAssignInstruction(instruction);
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
			if (!operationStack_.isEmpty()) {
				auto value = operationStack_.pop();
				std::cout << value.number << std::endl;
			}
		}
		else if (op == "print_s") {
			if (!operationStack_.isEmpty()) {
				auto value = operationStack_.pop();
				std::cout << value.word << std::endl;
			}
		}
		else if (op == "call") {

			if (instruction.operands_[0].value().find(".") != std::string::npos) {
				setState(State::CLASS);
			}

			// 存入当前地址并跳转
			addressStack_.push(index_);
			size_t offset = functionTable_.find(instruction.operands_[0].value())->second;
			index_ = offset;
		}
		else if (op == "ret") {
			// 跳回调用地址
			size_t offset = addressStack_.pop();
			index_ = offset;
			// 将状态重置
			setState(State::VALUE);	
		}
		else if (op == "create_class") {
			auto classType = instruction.operands_[0].value();
			auto className = instruction.operands_[1].value();

			setState(State::CLASS);
			// 存入当前地址并跳转
			addressStack_.push(index_);
			index_ = functionTable_.find(classType)->second;
			currentClass_ = className;		
		}
		else if (op == "end_class") {
			setState(State::VALUE);
			// 跳回调用地址
			size_t offset = addressStack_.pop();
			index_ = offset;
		}
		else if (op == "bind") {
			// 将函数与类实例绑定
			auto functionName = instruction.operands_[0].value();
			size_t index = functionTable_.find(functionName)->second;
			
			// 绑定类名
			functionName = currentClass_ + "." + functionName;
			functionTable_.insert({functionName, index });
		}
		else {
			;
		}
	}

	void Machine::handlePushInstruction(const Instruction &instruction) {
		if (instruction.operands_[0].type() == TokenType::VARIABLE) {
			handlePushVariableInstruction(instruction);
		}
		else if (instruction.operands_[0].type() == TokenType::CHAR) {
			auto value = new Value{ instruction.operands_[0].value()[0] };
			pushVariable(value);
		}
		else if (instruction.operands_[0].type() == TokenType::NUMBER) {
			auto value = new Value{ std::stoi(instruction.operands_[0].value()) };
			pushVariable(value);
		}
		else {
			;	// error
		}
	}

	void Machine::handlePushVariableInstruction(const Instruction &instruction) {
		auto name = instruction.operands_[0].value();
		if (state_ == State::CLASS) {
			bindClass(name);
		}
		if (!hasVariable(name)) {
			std::cout << "NameError: " << "name '" << name << "' is not defined" << std::endl;
		}
		else {
			auto value = findVariable(name);
			if (state_ == State::INDEX) {
				pushArray(value);
			}
			else {
				pushVariable(value);
			}
		}
	}

	void Machine::handleAssignInstruction(const Instruction &instruction) {
		if (state_ == State::ARRAY) {
			handleAssignArrayInstruction(instruction);
		}
		else if (state_ == State::INDEX) {
			handleAssignArrayElementInstruction(instruction);
		}
		else {
			handleAssignVariableInstruction(instruction);
		}
	}

	void Machine::handleAssignArrayInstruction(const Instruction &instruction) {
		// 创建/替换数组
		std::string name = instruction.operands_[0].value();
		if (state_ == State::CLASS) {
			name = bindCurrentClass(name);
		}

		auto size = operationStack_.pop().number;
		auto valuePtr = createArray(size);
		setArray(name, valuePtr, size);
		setState(State::VALUE);
	}

	void Machine::handleAssignArrayElementInstruction(const Instruction &instruction) {
		// 修改数组元素值
		std::string name = instruction.operands_[0].value();
		auto index = operationStack_.pop().number;
		auto valuePtr = createValue(operationStack_.pop().number);
		*(findVariable(name) + index) = *valuePtr;
		setState(State::VALUE);
	}

	void Machine::handleAssignVariableInstruction(const Instruction &instruction) {
		// 创建/替换变量
		std::string name = instruction.operands_[0].value();
		if (state_ == State::CLASS) {
			name = bindCurrentClass(name);
		}

		auto valuePtr = createValue(operationStack_.pop().number);
		setVariable(name, valuePtr);
	}


}