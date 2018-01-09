#include "stdafx.h"
#include "Machine.h"

namespace Guilmon {
	void Machine::execute(Instruction& instruction) {
		auto op = instruction.op_;
		if (op == "push") {
			stack_.push(instruction.values_[0]);
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
			;
		}
		else {
			;
		}
	}
}

