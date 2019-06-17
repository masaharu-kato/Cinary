#include <memory>
#include <type_traits>
#include <stdexcept>
#include "TypeOf.h"

namespace cinary {

//	任意型クラス（一度構築すると型は固定、無効値も持てる）
	class FixedAny {
	private:

	//	データ保管用基底クラス
		class PlaceHolder {
		public:
		//	型情報の取得
			virtual const std::type_info& getType() const noexcept = 0;
		};

	//	データ保管クラス
		template <class T>
		class Holder : public PlaceHolder {
		private:
		//	データ
			T val;

		public:	
		//	データの直接構築
			template <class... Args>
			Holder(Args&&... args)
				: val(args...)
			{}

		//	変更可能なデータを取得
			template <class Type>
			operator Type&() noexcept
			{
				return val;
			}

		//	読み取り用のデータを取得
			template <class Type>
			operator const Type&() const noexcept
			{
				return val;
			}

		//	型情報の取得
			const std::type_info& getType() const noexcept override {
				return typeid(T);
			}
		};

	//	データ保管変数
		std::unique_ptr<PlaceHolder> context;
	
	public:
	//	無効値として構築
		FixedAny() noexcept = default;

	//	構築済みの任意の型から構築
		template <class Type>
		FixedAny(const Type& type) noexcept
		{
			context.reset(new Holder<Type>(type));
		}

	//	1つ目の引数に指定した型を2つ目以降の引数を用いて直接構築
		template <class Type, class... Args>
		FixedAny(TypeOf<Type> type, Args&&... args) noexcept
		{
			context.reset(new Holder<Type>(args...));
		}

	//	(静的関数) Typeの型を直接構築
		template <class Type, class... Args>
		static auto create(Args&&... args) noexcept
		{
			return new FixedAny(TypeOf<Type>(), args...);
		}

	//	無効値であれば偽を返す
		explicit operator bool() const
		{
			return (bool)context;
		}

	//	変更可能なデータの取得（指定した型に変換できない場合は例外を送出）
		template <class Type>
		operator Type&()
		{
			if(!context) throw std::runtime_error("This value is not available.");
			return dynamic_cast<Holder<Type>&>(*context.get());
		}

	//	読み取り用データの取得（指定した型に変換できない場合は例外を送出）
		template <class Type>
		operator const Type&() const
		{
			if(!context) throw std::runtime_error("This value is not available.");
			return dynamic_cast<const Holder<Type>&>(*context.get());
		}

	//	OR演算子による既定値の設定
		template <class Type>
		FixedAny operator |(const Type& defval) const {
			if(*this) return *this;
			return defval;
		}

	};

}