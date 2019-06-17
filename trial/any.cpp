#include <memory>
#include <vector>
#include <iostream>

class Dwalin {
public:
     void show_yourself() const
    { std::cout << "Dwalin at your service!" << std::endl ; }
};

class Balin {
public:
     void show_yourself() const
    { std::cout << "Balin at your service!" << std::endl ; }
};

class Kili {
public:
     void show_yourself() const
    { std::cout << "Kili at your service!" << std::endl ; }
} ;

class Any {
private:
    class PlaceHolder {
    public:
        virtual ~PlaceHolder(){}
        virtual PlaceHolder * clone(void) = 0;
    } ;

    template <class Type>
    class Holder : public PlaceHolder {
    public:
        explicit Holder(Type const & dwarf)
            : held(dwarf)
        {}

        PlaceHolder * clone(void) override
        {
            return new Holder(held) ;
        }

    private:
        Type held;
    } ;

public :
    template <class Type>
    Any(const Type& dwarf)
        : content(new Holder<Type>(dwarf))
    {}

    Any(const Any& other)
        : content(other.content.get() ? other.content->clone() : 0)
    { }

    Any& operator = (const Any& rhs)
    {
        if(this != &rhs) content.reset(rhs.content->clone());
        return *this;
    }

    template <class Type>
	Any& operator = (const Type& rhs)
    {
        content.reset(new Holder<Type>(rhs)) ;
        return *this;
	}

private :
    std::unique_ptr<PlaceHolder> content;
};


int main() {
    std::vector<Any> dwarfs;

    for (int i = 0 ; i != 20 ; ++i) {
        switch(rand()%3) {
        case 0 :
            dwarfs.push_back(Dwalin()) ;
            break ;
        case 1 :
            dwarfs.push_back(Balin()) ;
            break ;
        case 2 :
            dwarfs.push_back(Kili()) ;
            break ;
        }
    }

    for(const auto& dwarf : dwarfs) dwarf.show_yourself() ;
    
}

