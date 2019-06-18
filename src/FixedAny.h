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

		//	const�C�����ǂ���
			virtual bool isConst() const noexcept = 0;


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

		//	const�C�����ǂ���
			virtual bool isConst() const noexcept = 0 {
				return std::is_const<Type>::value;
			}
		};

	//	�f�[�^�ۊǕϐ�
		std::unique_ptr<PlaceHolder> context;

		
	//	�f�[�^�ۊǕϐ����w�肵���^�ɕϊ��icontext �������l�̏ꍇ��ϊ��ł��Ȃ��ꍇ�͗�O�𑗏o�j
		template <class _T>
		_T& cast_context()
		{
			return dynamic_cast<Holder<_T>&>(*context.get());
		}
		
	//	�f�[�^�ۊǕϐ����w�肵���^�ɕϊ��icontext �������l�̏ꍇ��ϊ��ł��Ȃ��ꍇ�͗�O�𑗏o�j
		template <class _T>
		const _T& cast_context() const
		{
			return dynamic_cast<const Holder<_T>&>(*context.get());
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


	//	�f�[�^�ۊǕϐ����w�肵���^�Ŏ擾�i�ł��Ȃ��ꍇ�͗�O�𑗏o�j
		template <class T>
		T& getContext()
		{
		//	�����l�̏ꍇ��O�𑗏o
			if(!context) throw std::runtime_error("This value is not available.");
			
		//	�ۊǂ��Ă���^�̎Q�Ƃ�const�̗L���ɂ���ăL���X�g��̌^��ς���
			if(context->isReference()){
				if(context->isConst()) {
					return cast_context<const T&>();
				}else{
					return cast_context<T&>();
				}
			}

			if(context->isConst()) return cast_context<const T>();
			
			return cast_context<T>();
		}

	//	�f�[�^�ۊǕϐ����w�肵���^�Ŏ擾�i�ł��Ȃ��ꍇ�͗�O�𑗏o�j
		template <class T>
		const T& getContext() const
		{
		//	�����l�̏ꍇ��O�𑗏o
			if(!context) throw std::runtime_error("This value is not available.");
			
		//	�ۊǂ��Ă���^�̎Q�Ƃ�const�̗L���ɂ���ăL���X�g��̌^��ς���
			if(context->isReference()){
				if(context->isConst()) {
					return cast_context<const T&>();
				}else{
					return cast_context<T&>();
				}
			}

			if(context->isConst()) return cast_context<const T>();
			
			return cast_context<T>();
		}


	//	�ύX�\�ȃf�[�^�̎擾�i�w�肵���^�ɕϊ��ł��Ȃ��ꍇ�͗�O�𑗏o�j
		template <class Type>
		operator Type&()
		{
			return getContext<Type>();
		}

	//	�ǂݎ��p�f�[�^�̎擾�i�w�肵���^�ɕϊ��ł��Ȃ��ꍇ�͗�O�𑗏o�j
		template <class Type>
		operator const Type&() const
		{
			return getContext<Type>();
		}


	//	�����l�ł���΋U��Ԃ�
		explicit operator bool() const
		{
			return (bool)context;
		}


	//	OR���Z�q�ɂ�����l�̐ݒ�
		template <class Type>
		FixedAny operator |(const Type& defval) const {
			if(*this) return *this;
			return defval;
		}

	};

}