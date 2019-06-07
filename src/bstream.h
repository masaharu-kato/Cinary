#include <iostream>
#include <memory>
#include "type_utils.h"

namespace Cinary {

	class BinaryStream {
	public:
		using Byte = char;
	};

	class InputBinaryStream : public BinaryStream {
	public:
		using This = InputBinaryStream;
		using ISPtr = std::shared_ptr<std::istream>;

	private:
		ISPtr is;

	public:
		InputBinaryStream(ISPtr is)
			: is(is) {}

		template <class Type>
		This& operator >>(const Type& obj) {
			is->read((Byte*)&obj, sizeof(obj));
			return *this;
		}
	};


	class OutputBinaryStream : public BinaryStream {
	public:
		using This = OutputBinaryStream;
		using OSPtr = std::shared_ptr<std::ostream>;

	private:
		OSPtr os;

		template <class Type>
		void writeType(size_t n = 0);

		template <class Type>
		void writeValue(Type obj)
		{
			os->write((Byte*)&obj, sizeof(obj));
		}

		template <class Ptr>
		void writePtr(Ptr ptr);

	public:
		OutputBinaryStream(OSPtr os)
			: os(os) {}

		template <class... Args>
		This& operator <<(Args... args)
		{
			write(args...);
			return *this;
		}

		template <class Type>
		void write(Type obj)
		{
			if(!is_container<Type>::value)
			{
				writeType<Type>();
				writeValue(obj);
			}
			else{
				write(obj.begin(), obj.end());
			}
		}
		
		template <class T> void write(std::unique_ptr<T> ptr){ writePtr(ptr); }
		template <class T> void write(std::shared_ptr<T> ptr){ writePtr(ptr); }
		template <class T> void write(T* ptr){ writePtr(ptr); }

		template <class CItr>
		void write(CItr ibeg, CItr iend)
		{
			writeType<decltype(*ibeg)>(std::distance(ibeg, iend));
			for(CItr itr = ibeg; itr != iend; ++itr) {
				writeValue(*itr);
			}
		}


	};

}