#pragma once
#include <iostream>
#include <memory>
#include "type_utils.h"

namespace cinary {

	class BinaryStream {
	public:
		using Byte = char;
		using Size = unsigned long int;
		using Char = char;
		using TypeID = Size;
		using DataID = Size;
	};

}