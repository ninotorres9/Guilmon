// Guilmon.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Machine.h"
#include "Parser.h"

#include <fstream>

using namespace Guilmon;

void test() {
	std::string filename = "E:\\code\\Impmon_copy\\demo.gl";
	std::ifstream infile(filename);
	std::string text;
	std::string temp;
	while (std::getline(infile, temp)) {
		text.append(temp);
	}
	Parser parser(text);
	Machine machine(parser.getInstructions());
	machine.run();
	system("pause");
}


int main(int argc, char *argv[])
{
	// test();

	if (argc > 2) {
		auto tag = argv[1]; // tag = -r (run)
		auto filename = std::string(argv[2]);
		std::ifstream infile(filename);

		std::string text;
		std::string temp;
		while (std::getline(infile, temp)) {
			text.append(temp);
		}

		Parser parser(text);
		Machine machine(parser.getInstructions());
		machine.run();
	}
	else {
		std::cout << "no comment" << std::endl;
	}

	
    return 0;
}


