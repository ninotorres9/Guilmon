

#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <memory>

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
		inline bool isEmpty() const{
			return container_.size() == 0;
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

	union Value {
		int number;
		char word;
		~Value() { ; }
	};

	
	class ClassType{
	public:
		std::map<std::string, Value*> variableTable_;
		std::map<std::string, size_t> functionTable_;
	};

	//state_ == class;
	//std::map<std::string>, ClassType*> classTable_;


	class Machine {
	public:
		Machine(const std::vector<Instruction> &instructions) :
			instructions_(instructions), index_(0), state_(State::VALUE) {
			;
		}
		inline void run() {

			// 第一趟扫描
			size_t index = 0;
			for (auto inst : instructions_) {
				if (inst.op_ == "tag")
					functionTable_.insert({ inst.operands_[0].value(), index });
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
		inline Value* createArray(unsigned int size) {
			auto ptr = alloc_.allocate(sizeof(Value) * size);
			return ptr;
		}
		template<typename T> inline Value* createValue(T value) {
			auto ptr = alloc_.allocate(sizeof(Value));
			alloc_.construct(ptr, Value{ value });
			return ptr;
		}
		inline void createVariable(const std::string& name, Value* value) {
			variableTable_.insert({ name, value });
		}
		inline Value* findVariable(const std::string& name) const {
			return variableTable_.find(name)->second;
		}
		inline bool hasVariable(const std::string& name) const {
			return variableTable_.find(name) != variableTable_.end();
		}
		inline void setVariable(const std::string& name, Value* value) {
			if (variableTable_.find(name) != variableTable_.end()) {
				variableTable_.find(name)->second = value;
			}
			else {
				createVariable(name, value);
			}
		}
		inline void setArray(const std::string& name, Value* arrayPtr, size_t size) {
			if (variableTable_.find(name) != variableTable_.end()) {
				;
			}
			else {
				createVariable(name, arrayPtr);
				for (size_t offset = 0; offset != size; ++offset) {
					auto value = operationStack_.pop();
					alloc_.construct(arrayPtr + offset, value);
				}
			}
		}
		inline void deleteVariable(const std::string& name) {
			variableTable_.erase(name);
		}
		inline void bindClass(std::string& name) {
			name = currentClass_ + "." + name;
		}
		inline std::string bindCurrentClass(const std::string& name) {
			return currentClass_ + "." + name;
		}
	private:
		void handlePushInstruction(const Instruction &instruction);
		void handlePushVariableInstruction(const Instruction &instruction);
		void handleAssignInstruction(const Instruction &instruction);
		void handleAssignArrayInstruction(const Instruction &instruction);
		void handleAssignArrayElementInstruction(const Instruction &instruction);
		void handleAssignVariableInstruction(const Instruction &instruction);
		inline void pushVariable(Value* value) {
			operationStack_.push(*value);
		}
		inline void pushArray(Value* value) {
			auto offset = operationStack_.pop().number;
			operationStack_.push(*(value + offset));
			setState(State::VALUE);
		}

		

	private:
		enum class State {
			VALUE,
			ARRAY,
			INDEX,
			CLASS,
		};

	private:
		inline void setState(State state) {
			state_ = state;
		}
	private:
		std::vector<Instruction> instructions_;
		size_t index_;
		Stack<Value> operationStack_;	
		Stack<size_t> addressStack_;	
		std::allocator<Value> alloc_;		// 内存池
		std::map<std::string, Value*> variableTable_;
		std::map<std::string, size_t> functionTable_;
		std::map<std::string, ClassType*> classTable_;
		std::string currentClass_;
		State state_;
	};




}

