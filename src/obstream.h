#pragma once
#include "bstream.h"
#include "FixedAny.h"

namespace cinary {
	
//	バイナリ出力ストリーム
	class OutputBinaryStream : public BinaryStream {
	public:
		using This = OutputBinaryStream;
		using OSPtr = std::shared_ptr<std::ostream>;

	private:
	//	標準出力ストリームのポインタ
		OSPtr os;

	public:
		OutputBinaryStream(OSPtr os)
			: os(os) {}

	//	データの書き込み
		//template <class T>
		//void write(const char* name, const T& val);

	//	初期化子リストによるデータの書き込み
		void write(std::initializer_list<std::pair<const char*, FixedAny>>&);

		//template <class First, class... Rests>
		//void write(const First& first, Rests&&... rests)
		//{
		//	write(first);
		//	wirte(rests...);
		//}

	private:

	//	型情報の書き込み
		template <class Type>
		void writeType(size_t n = 0);

	//	
		template <class Type>
		void writeValue(const Type& obj)
		{
			os->write((Byte*)&obj, sizeof(obj));
		}

		template <class Ptr>
		void writePtr(Ptr ptr);
		

	protected:

		template <class Type>
		void write(const Type& obj)
		{
			if(is_container<Type>::value) return writeContainer(obj);
			
		//	writeType<Type>();
			writeValue(obj);
		}

		template <class First, class Second>
		void write(std::pair<First, Second> pair)
		{
			writeType<decltype(pair)>();
			writeValue(pair.first);
			writeValue(pair.second);
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

	using OBStream = OutputBinaryStream;

}