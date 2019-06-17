#include <iostream>
#include <unordered_map>
#include <string>

int main(void) {

	std::unordered_map<std::string, int> data = {
		{"hoge", 15434},
		{"piyopiyo", -424},
		{"foobar", 8547834},
	};

	for(const auto& cdata : data) {
		std::cout << cdata.first << ": " << cdata.second << "\n";
	}

	return 0;
}