#pragma once
#include <ExampleLibrary2.h>

class Robot {
public:
	Robot() {};
	~Robot() {};

	ExampleLibrary2 lib;

	void setup();
	void loop();
};
