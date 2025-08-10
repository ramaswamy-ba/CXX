#include <iostream>
#include <memory>
#include <typeinfo>

class Any
{
    struct Base
    {
        virtual ~Base() = default;
        virtual const std::type_info& type() const = 0;
        virtual std::unique_ptr<Base> clone() const = 0;
    };

    template<typename T>
    struct Holder : Base
    {
        T value;
        Holder(T&& val) : value(std::forward<T>(val)) {}

        template<typename U>
        Holder(U&& val) : value(std::forward<U>(val)) {}

        const std::type_info& type() const override { return typeid(T); }

        std::unique_ptr<Base> clone() const override { return std::make_unique<Holder<T>>(value); }
    };

    std::unique_ptr<Base> ptr;

public:
    Any() = default;

    template<typename T>
    Any(T&& val) : ptr(std::make_unique<Holder<T>>(std::forward<T>(val))) {}

    Any(const Any& other) : ptr(other.ptr ? other.ptr->clone() : nullptr) {}

    Any& operator=(const Any& other)
    {
        if (this != &other)
            ptr = other.ptr ? other.ptr->clone() : nullptr;
        return *this;
    }

    const std::type_info& type() const { return ptr ? ptr->type() : typeid(void); }

    template<typename T>
    T& cast()
    {
        if (type() != typeid(T))
            throw std::bad_cast();
        return static_cast<Holder<T>*>(ptr.get())->value;
    }
};

int main()
{
    Any a = 42;
    std::cout << "Stored type: " << a.type().name() << "\n";
    std::cout << "Value: " << a.cast<int>() << "\n";

    a = std::string("Hello");
    std::cout << "Stored type: " << a.type().name() << "\n";
    std::cout << "Value: " << a.cast<std::string>() << "\n";
}
