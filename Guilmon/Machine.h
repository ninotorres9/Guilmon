

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

	union Value {
		int number;
		char word;
		~Value() { ; }
	};

	class Machine {
	public:
		Machine(const std::vector<Instruction> &instructions) :
			instructions_(instructions) , index_(0){
			;
		}
		inline void run() {

			// ��һ��ɨ��
			size_t index = 0;
			for (auto inst : instructions_) {
				if (inst.op_ == "tag")
					functionTable_.insert({ inst.operands_[0].value(), index});
				index++;
			}

			// ��main������
			auto main = functionTable_.find("main");
			if (main != functionTable_.end()) {
				index_ = main->second;

				// ���ﲻ��for each��Ϊ�˷���jmp��������ת
				while (index_ < instructions_.size()) {
					execute();
				}
			}
			else {
				// ����û��main
			}


		}
	private:
		void execute();
		

		inline Value* createArray(unsigned int size) {
			auto ptr = alloc_.allocate(sizeof(Value) * size);
			return ptr;
		}

		template<typename T>
		inline Value* createValue(T value) {
			auto ptr = alloc_.allocate(sizeof(Value));
			alloc_.construct(ptr, Value{ value });
			return ptr;
		}
		inline Value* getValue(const std::string& name) {
			return variableTable_.find(name)->second;
		}
	private:
		std::vector<Instruction> instructions_;
		size_t index_;
		Stack<Value> operationStack_;	// ����ջ
		Stack<size_t> addressStack_;	// ��ַջ

		std::allocator<Value> alloc_;		// �ڴ��
		std::map<std::string, Value*> variableTable_;
		std::map<std::string, size_t> functionTable_;
	};

}

