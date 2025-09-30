#include <iostream>
#include <format>

#include <Robot.h>

#include "Main.h"

int main(int argc, char **argv) {
	(void)argc;
	(void)argv;
	Robot robot;
	
	robot.setup();
	while (true) {
		robot.loop();
	}
	return 0;
}
