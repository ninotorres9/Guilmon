

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


	//class LocalScope {
	//public:
	//	inline void createVariable(const std::string& name, Value* value) {
	//		variableTable_.insert({ name, value });
	//	}
	//	inline Value* findVariable(const std::string& name) const {
	//		return variableTable_.find(name)->second;
	//	}
	//	inline void setVariable(const std::string& name, Value* value) {
	//		variableTable_.find(name)->second = value;
	//	}
	//	inline void deleteVariable(const std::string& name) {
	//		auto ptr = variableTable_.find(name);
	//		variableTable_.erase(ptr);
	//	}
	//public:
	//	std::map<std::string, Value*> variableTable_;
	//};

	/*
		new Scope
		开一个for循环
		scope.inScope() -> 创建并跳转
		create
		find
		set
		delete
		scope.outScope() -> 删除并退出
		
	*/

	class BaseScope;

	using BaseScopePtr = std::shared_ptr<BaseScope>;

	class BaseScope {
	public:
		BaseScope(BaseScopePtr parentScope = nullptr) : parent_(parentScope){

		}
		BaseScopePtr parent_;
		std::vector<BaseScopePtr> children_;
		std::map<std::string, Value*> variableTable_;
	};

	class Scope {
	public:
		Scope() {
			currentScope_ = std::make_shared<BaseScope>();
		}
	private:
		BaseScopePtr currentScope_;

	private:
		inline void addChild() {
			auto child = std::make_shared<BaseScope>(currentScope_);
			currentScope_->children_.push_back(child);
		}
		inline void enterNewChild(){
			size_t size = currentScope_->children_.size();
			currentScope_ = currentScope_->children_[size - 1];
		}
		inline void returnParent() {
			currentScope_ = currentScope_->parent_;
			// 删除原来的子作用域
			currentScope_->children_.pop_back();
		}
		inline Value* findVariable(const std::string& name, BaseScopePtr scope) const {
			auto result = scope->variableTable_.find(name);
			/*
			首先在当前scope搜索，无结果转向parent scope继续搜索，
			均无结果返回nullptr
			*/
			if (result != scope->variableTable_.end())
				return result->second;
			else if (currentScope_->parent_ != nullptr)
				return findVariable(name, currentScope_->parent_);
			else
				return nullptr;
		}
	public:
		inline void addSubScope() {
			// 创建并进入子作用域
			addChild();
			enterNewChild();
		}
		inline void exitScope() {
			// 返回上一层作用域
			returnParent();
		}
		inline void createVariable(const std::string& name, Value* value) {
			currentScope_->variableTable_.insert({ name, value });
		}
		inline Value* findVariable(const std::string& name) const {
			return findVariable(name, currentScope_);
		}
		inline void setVariable(const std::string& name, Value* value) {
			currentScope_->variableTable_.find(name)->second = value;
		}
		inline void deleteVariable(const std::string& name) {
			auto ptr = currentScope_->variableTable_.find(name);
			currentScope_->variableTable_.erase(ptr);
		}
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
			variableTable_.createVariable(name, value);
		}
		inline Value* findVariable(const std::string& name) const {
			return variableTable_.findVariable(name);
		}
		inline void setVariable(const std::string& name, Value* value) {
			variableTable_.setVariable(name, value);
		}
		inline void deleteVariable(const std::string& name) {
			variableTable_.deleteVariable(name);
		}
		
	private:
		std::vector<Instruction> instructions_;
		size_t index_;
		Stack<Value> operationStack_;	
		Stack<size_t> addressStack_;	

		std::allocator<Value> alloc_;		// 内存池
		// LocalScope variableTable_;
		Scope variableTable_;
		std::map<std::string, size_t> functionTable_;
	};

}

