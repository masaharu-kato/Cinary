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

		//	�Q�ƌ^���ǂ���
			virtual bool isReference() const noexcept = 0;

		//	const���ǂ���


		};

	//	�f�[�^�ۊǃN���X
		template <class Type>
		class Holder : public PlaceHolder {
		private:
		//	�f�[�^
			Type val;

		public:	
		//	�f�[�^�̒��ڍ\�z
			template <class... Args>
			Holder(Args&&... args)
				: val(args...)
			{}

		//	�ύX�\�ȃf�[�^���擾
			template <class T>
			operator T&() noexcept
			{
				return val;
			}

		//	�ǂݎ��p�̃f�[�^���擾
			template <class T>
			operator const T&() const noexcept
			{
				return val;
			}

		//	�^���̎擾
			const std::type_info& getType() const noexcept override {
				return typeid(T);
			}

		//	�Q�ƌ^���ǂ���
			virtual bool isReference() const noexcept = 0 {
				return std::is_reference<Type>::value;
			}
		};

	//	�f�[�^�ۊǕϐ�
		std::unique_ptr<PlaceHolder> context;


	//	�f�[�^�ۊǕϐ����w�肵���^�ɕϊ��i�ł��Ȃ��ꍇ�͗�O�𑗏o�j
		template <class T>
		T cast_context() const
		{
			if(!context) throw std::runtime_error("This value is not available.");
			return dynamic_cast<T>(*context.get());
		}

	
	public:
	//	�����l�Ƃ��č\�z
		FixedAny() noexcept = default;

	//	�\�z�ς݂̔C�ӂ̌^����\�z
		template <class T>
		FixedAny(const T& type) noexcept
		{
			context.reset(new Holder<T>(type));
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
		//	�����l�̏ꍇ��O�𑗏o
			if(!context) throw std::runtime_error("This value is not available.");
			
		//	�ۊǂ��Ă���^���Q�ƌ^�̏ꍇ���̂܂ܕԂ�
			if(context->isReference()) return dynamic_cast<Holder<Type&>&>(*context.get());
			
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