#pragma once

#include "Parser.h"
#include <functional>

namespace Guilmon {

	bool isDigit(const std::string& text) {
		return std::all_of(text.begin(), text.end(), isdigit);
	}

	bool isConstant(Instruction &instruction) {
		return instruction.op_ == "push" &&
			instruction.operands_[0].type() == TokenType::NUMBER;
	}

	//std::string sum(const std::string &lhs, const std::string &rhs) {
	//	return std::to_string(std::stoi(lhs) + std::stoi(rhs));
	//}

	class Optimizer {
	public:
		Optimizer(const std::vector<Instruction> &instructions) :instructions_(instructions) {
			constantFolding();
		}
		~Optimizer() { ; }
		inline std::vector<Instruction> getInstructions() {
			return instructions_;
		}
		inline void demo(std::vector<Instruction> &container, size_t index, const std::string& op, std::function<int(int, int)> calculate) {
			const size_t leftIndex = index - 2;
			const size_t rightIndex = index - 1;

			if (container[index].op_ == op) {
				if (isConstant(container[leftIndex]) &&
					isConstant(container[rightIndex])) {
					auto lhs = std::stoi(container[leftIndex].operands_[0].value());
					auto rhs = std::stoi(container[rightIndex].operands_[0].value());

					// 添加删除标记
					container[leftIndex] = Instruction("for_delete", {});
					container[rightIndex] = Instruction("for_delete", {});

					// 将二元操作指令更改为常量
					container[index] = Instruction("push",
						{ Token(TokenType::NUMBER, std::to_string(calculate(lhs, rhs))) });
				}
			}
		}
		inline void constantFolding() {
			
			for (size_t i = 0; i != instructions_.size(); ++i) {
				demo(instructions_, i, "add", [](int lhs, int rhs) {return lhs + rhs; });
				demo(instructions_, i, "sub", [](int lhs, int rhs) {return lhs - rhs; });
			}

			// 删除标记语句
			instructions_.erase(
				std::remove_if(instructions_.begin(), instructions_.end()
					, [](Instruction inst) {return inst.op_ == "for_delete"; }),
				instructions_.end()
			);
			
		}
	private:
		std::vector<Instruction> instructions_;
	};
}