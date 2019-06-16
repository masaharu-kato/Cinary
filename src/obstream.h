#pragma once
#include "bstream.h"

namespace cinary {
	
//	バイナリ出力ストリーム
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

		template <class First, class... Rests>
		void write(const First& first, Rests&&... rests)
		{
			write(first);
			wirte(rests...);
		}


		template <class Type>
		void write(const Type& obj)
		{
			if(is_container<Type>::value) return writeContainer(obj);
			
		//	writeType<Type>();
			writeValue(obj);
		}
		
		template <class T> void write(std::unique_ptr<T> ptr){ writePtr(ptr); }
		template <class T> void write(std::shared_ptr<T> ptr){ writePtr(ptr); }
		template <class T> void write(T* ptr){ writePtr(ptr); }

		template <class Type>
		void writeContainer(const Type& ct)
		{
			writeRange(ct.begin(), ct.end());
		}

		template <class CItr>
		void writeRange(CItr ibeg, CItr iend)
		{
		//	writeType<decltype(*ibeg)>(std::distance(ibeg, iend));
			for(CItr itr = ibeg; itr != iend; ++itr) {
				writeValue(*itr);
			}
		}

	};

}