#pragma once
#include "bstream.h"
#include <vector>

namespace cinary {
	
//	バイナリ入力ストリーム
	class InputBinaryStream : public BinaryStream {
	public:
		using This = InputBinaryStream;
		using ISPtr = std::shared_ptr<std::istream>;

	private:
		ISPtr is;

	public:
		InputBinaryStream(ISPtr is)
			: is(is) {}

	protected:
		template <class Type>
		Type read() {
			Type val;
			is->read((Byte*)&val, sizeof(val));
			return val;
		}

		
		template <class Type>
		void readTo(Type& out, Size size) {
			is->read((Byte*)&out, sizeof(out) * size);
		}
		
		template <>
		std::string read<std::string>() {
			auto size = read<Size>();
			std::string str;
			str.resize(size);
			readTo(*str.data(), size);
			return str;
		}
		
		//template <class Type>
		//template <>
		//auto read<std::vector<Type>>() {
		//	auto size = read<Size>();
		//	std::vector<Type> vec;
		//	vec.resize(size);
		//	readTo(*vec.data(), size);
		//	return vec;
		//}


		//template <class Type>
		//This& operator >>(const Type& obj) {
		//	is->read((Byte*)&obj, sizeof(obj));
		//	return *this;
		//}

	};

}