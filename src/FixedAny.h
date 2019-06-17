#include <memory>
#include <type_traits>
#include <stdexcept>
#include "TypeOf.h"

namespace cinary {

//	�C�ӌ^�N���X�i��x�\�z����ƌ^�͌Œ�A�����l�����Ă�j
	class FixedAny {
	private:

	//	�f�[�^�ۊǗp���N���X
		class PlaceHolder {
		public:
		//	�^���̎擾
			virtual const std::type_info& getType() const noexcept = 0;
		};

	//	�f�[�^�ۊǃN���X
		template <class T>
		class Holder : public PlaceHolder {
		private:
		//	�f�[�^
			T val;

		public:	
		//	�f�[�^�̒��ڍ\�z
			template <class... Args>
			Holder(Args&&... args)
				: val(args...)
			{}

		//	�ύX�\�ȃf�[�^���擾
			template <class Type>
			operator Type&() noexcept
			{
				return val;
			}

		//	�ǂݎ��p�̃f�[�^���擾
			template <class Type>
			operator const Type&() const noexcept
			{
				return val;
			}

		//	�^���̎擾
			const std::type_info& getType() const noexcept override {
				return typeid(T);
			}
		};

	//	�f�[�^�ۊǕϐ�
		std::unique_ptr<PlaceHolder> context;
	
	public:
	//	�����l�Ƃ��č\�z
		FixedAny() noexcept = default;

	//	�\�z�ς݂̔C�ӂ̌^����\�z
		template <class Type>
		FixedAny(const Type& type) noexcept
		{
			context.reset(new Holder<Type>(type));
		}

	//	1�ڂ̈����Ɏw�肵���^��2�ڈȍ~�̈�����p���Ē��ڍ\�z
		template <class Type, class... Args>
		FixedAny(TypeOf<Type> type, Args&&... args) noexcept
		{
			context.reset(new Holder<Type>(args...));
		}

	//	(�ÓI�֐�) Type�̌^�𒼐ڍ\�z
		template <class Type, class... Args>
		static auto create(Args&&... args) noexcept
		{
			return new FixedAny(TypeOf<Type>(), args...);
		}

	//	�����l�ł���΋U��Ԃ�
		explicit operator bool() const
		{
			return (bool)context;
		}

	//	�ύX�\�ȃf�[�^�̎擾�i�w�肵���^�ɕϊ��ł��Ȃ��ꍇ�͗�O�𑗏o�j
		template <class Type>
		operator Type&()
		{
			if(!context) throw std::runtime_error("This value is not available.");
			return dynamic_cast<Holder<Type>&>(*context.get());
		}

	//	�ǂݎ��p�f�[�^�̎擾�i�w�肵���^�ɕϊ��ł��Ȃ��ꍇ�͗�O�𑗏o�j
		template <class Type>
		operator const Type&() const
		{
			if(!context) throw std::runtime_error("This value is not available.");
			return dynamic_cast<const Holder<Type>&>(*context.get());
		}

	//	OR���Z�q�ɂ�����l�̐ݒ�
		template <class Type>
		FixedAny operator |(const Type& defval) const {
			if(*this) return *this;
			return defval;
		}

	};

}