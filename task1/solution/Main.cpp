#include <iostream>
#include <format>

#include "Main.h"

int main(int argc, char **argv) {
	(void)argc;
	(void)argv;
	ExampleLibrary lib;
	
	std::cout << "Hello, world!" << std::endl;
	std::cout << std::format("exampleFunction(3)={:.4f}", lib.exampleFunction(3)) << std::endl;
	return 0;
}
