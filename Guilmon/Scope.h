#pragma once

#include "Machine.h"

/*
Scope用作嵌套作用域，已从虚拟机中删除，留在这里是为了给以后别的项目用。

*/

namespace Guilmon {
	class BaseScope;

	using BaseScopePtr = std::shared_ptr<BaseScope>;

	class BaseScope {
	public:
		BaseScope(BaseScopePtr parentScope = nullptr) : parent_(parentScope) {

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
		inline void enterNewChild() {
			size_t size = currentScope_->children_.size();
			currentScope_ = currentScope_->children_[size - 1];
		}
		inline void returnParent() {
			currentScope_ = currentScope_->parent_;
			// 删除原来的子作用域
			currentScope_->children_.pop_back();
		}
		inline void setVariable(const std::string& name, Value* value, BaseScopePtr scope) {
			auto result = scope->variableTable_.find(name);

			if (result != scope->variableTable_.end())
				result->second = value;
			else if (scope->parent_ != nullptr)
				setVariable(name, value, scope->parent_);
			else
				;
		}
		inline Value* findVariable(const std::string& name, BaseScopePtr scope) const {
			auto result = scope->variableTable_.find(name);
			/*
			首先在当前scope搜索，无结果转向parent scope继续搜索，
			均无结果返回nullptr
			*/
			if (result != scope->variableTable_.end())
				return result->second;
			else if (scope->parent_ != nullptr)
				return findVariable(name, scope->parent_);
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
			setVariable(name, value, currentScope_);
		}
		inline void deleteVariable(const std::string& name) {
			auto ptr = currentScope_->variableTable_.find(name);
			currentScope_->variableTable_.erase(ptr);
		}
	};
}

