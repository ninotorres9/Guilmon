#include "pch.h"
#include "Parser.h"

using namespace Guilmon;
class ScopeTest : public ::testing::Test {
protected:
	std::allocator<Value> alloc_;
	template<typename T> 
	inline Value* createValue(T value) {
		auto ptr = alloc_.allocate(sizeof(Value));
		alloc_.construct(ptr, Value{ value });
		return ptr;
	}
};

TEST_F(ScopeTest, TestCreate) {
	Scope scope;
	auto value = createValue(Value{ 10 });
	scope.createVariable("var", value);

	EXPECT_EQ(scope.findVariable("var")->number, 10);
}

TEST_F(ScopeTest, TestSet) {
	Scope scope;
	auto value = createValue(Value{ 10 });
	scope.createVariable("var", value);
	value = createValue(Value{ 25 });
	scope.setVariable("var", value);

	EXPECT_EQ(scope.findVariable("var")->number, 25);
}

TEST_F(ScopeTest, TestDelete) {
	// 单纯删除table内的内容
	Scope scope;
	auto value = createValue(Value{ 10 });
	scope.createVariable("var", value);
	scope.deleteVariable("var");

	EXPECT_EQ(scope.findVariable("var"), nullptr);
}

TEST_F(ScopeTest, TestDelete_1) {
	// 删除子作用域变量，查找上层作用域同名变量
	Scope scope;
	auto value = createValue(Value{ 10 });
	scope.createVariable("var", value);

	scope.addSubScope();
	value = createValue(Value{ 25 });
	scope.createVariable("var", value);
	scope.deleteVariable("var");

	EXPECT_EQ(scope.findVariable("var")->number, 10);
}

TEST_F(ScopeTest, TestEnterScope) {
	Scope scope;
	auto value = createValue(Value{ 10 });
	scope.createVariable("var", value);

	scope.addSubScope();
	value = createValue(Value{ 25 });
	scope.createVariable("var", value);

	EXPECT_EQ(scope.findVariable("var")->number, 25);
}

TEST_F(ScopeTest, TestExitScope) {
	Scope scope;
	auto value = createValue(Value{ 10 });
	scope.createVariable("var", value);

	scope.addSubScope();
	value = createValue(Value{ 25 });
	scope.createVariable("var", value);

	// 返回上层作用域
	scope.exitScope();

	EXPECT_EQ(scope.findVariable("var")->number, 10);
}

TEST_F(ScopeTest, TestFind) {
	Scope scope;
	auto value = createValue(Value{ 10 });
	scope.createVariable("var", value);

	scope.addSubScope();
	value = createValue(Value{ 25 });
	scope.createVariable("bat", value);

	EXPECT_EQ(scope.findVariable("var")->number, 10);
}

TEST_F(ScopeTest, TestFind_1) {
	Scope scope;
	auto value = createValue(Value{ 10 });
	scope.createVariable("var", value);

	scope.addSubScope();
	scope.addSubScope();
	EXPECT_EQ(scope.findVariable("var")->number, 10);
}





