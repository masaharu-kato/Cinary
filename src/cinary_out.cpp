#include "cinary.h"
#include "bstream.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace Cinary;

class Test {};

int main()
{
	std::vector<Test> vec;

	OutputBinaryStream obs(std::make_shared<std::ofstream>("test.bin"));

//	obs << 1;
	obs << 1 << 175 << -435 << 0 << 3.14159 << 'F' << 'U' << 'G' << 'A';

	return 0;
}
