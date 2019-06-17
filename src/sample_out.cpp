#include "sample.h"
#include <iostream>
#include <fstream>

int main(void) {

	Hoge hoge;
	std::cin >> hoge;

	cinary::OBStream obs(std::make_shared<std::ofstream>("test.bin"));
	
	obs.write("hoge", hoge);

	return 0;
}
