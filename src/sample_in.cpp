#include "sample.h"
#include <iostream>
#include <fstream>

int main(void) {

	cinary::IBStream ibs(std::make_shared<std::ifstream>("test.bin"));

	Hoge hoge = ibs["hoge"] | Hoge(153, 3.1416, 'S');
	std::cout << hoge << "\n";

	return 0;
}