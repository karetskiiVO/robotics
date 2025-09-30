#include <iostream>
#include <format>

#include "Robot.h"

void Robot::setup() {
	std::cout << "Setting up" << std::endl;	
}

void Robot::loop() {
	for (double x = 0; x < 10; x += 0.001) {
		std::cout << std::format("f({:.3f})={:.3f}", x, lib.ExampleFunction2(x)) << std::endl;
	}
}
