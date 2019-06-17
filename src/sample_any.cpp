#include "FixedAny.h"
#include <iostream>

using namespace cinary;

int main(void) {

	using Type = std::pair<std::string, int>;

	{
	
	auto val = FixedAny(TypeOf<Type>(), "test", 1523);

	try {
		std::cout << (int)val << "\n";
	}
	catch(std::bad_cast& e) {
		std::cerr << e.what() << "\n";
	}

	((Type&)val).second = 16236;

	Type& typed_val = val;
	std::cout << typed_val.first << ", " << typed_val.second << "\n";
	
	}

	//{
	//
	//auto src = Type("source object", -1234);
	//auto refval = FixedAny(TypeOf<Type&>(), src);

	//((Type&)refval).first = "modified source object";

	//
	//Type& typed_refval = refval;
	//std::cout << typed_refval.first << ", " << typed_refval.second << "\n";

	//}

	return 0;
}