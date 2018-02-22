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
		auto tag = argv[1]; 

		// -r (run)
		if (tag[1] == 'r') { 
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
		// -t (tokens)
		else if (tag[1] == 't') {
			auto filename = std::string(argv[2]);
			std::ifstream infile(filename);

			std::string text;
			std::string temp;
			while (std::getline(infile, temp)) {
				text.append(temp);
			}
			Parser parser(text);
			parser.printTokens();
		}
		else {
			std::cout << "error comment: " << tag << std::endl;
		}

	}
	else {
		std::cout << "no comment" << std::endl;
	}

	
    return 0;
}


