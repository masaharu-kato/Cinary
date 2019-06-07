#include "cinary.h"
#include "bstream.h"
#include <iostream>
#include <fstream>

using namespace Cinary;

int main()
{
	InputBinaryStream ibs(std::shared_ptr<std::ifstream>(new std::ifstream("test.bin")));

	int v1, v2, v3, v4;
	double v5;
	char v6, v7, v8, v9;

	ibs >> v1 >> v2 >> v3 >> v4 >> v5 >> v6 >> v7 >> v8 >> v9;

	std::cout << v1 << ' ' << v2 << ' ' << v3 << ' ' << v4 << ' ' << v5 << ' ' << v6 << ' ' << v7 << ' ' << v8 << ' ' << v9;

	return 0;
}
