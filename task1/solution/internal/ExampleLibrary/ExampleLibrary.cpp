#include <cmath>
#include "ExampleLibrary.h"

double ExampleLibrary::exampleFunction(double arg) {
	return std::pow(std::sin(arg), 2) + std::cos(arg);
}
