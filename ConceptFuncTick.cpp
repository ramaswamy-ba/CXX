#include <iostream>
#include <memory>
#include <concepts>
#include <type_traits>

//Template functions support both overloading and specialization. Template classes only support specialization.
namespace rba
{
    template<class T>
    struct is_unique_ptr: std::false_type { };

    template<class T>
    struct is_unique_ptr <std::unique_ptr<T>> : std::true_type { };

    template<class T, typename = void>
    struct is_smart_ptr : std::false_type {};

    // Concept to check if a type is a smart pointer
    template <class T>
    concept SmartPointerType = std::is_same_v<T, std::unique_ptr<typename T::element_type>> || 
                               std::is_same_v<T, std::shared_ptr<typename T::element_type>>;

    template <typename T>
    struct is_smart_ptr<T, std::enable_if_t<SmartPointerType<T>>> : std::true_type {};

     template<typename T>
     using is_smart_ptr_v = is_smart_ptr<T>::value;
}

template <class T>
concept NonUniquePointer = std::is_pointer_v<T> || ( std::is_same_v<T, std::shared_ptr<typename T::element_type>> && !std::is_same_v<T, std::unique_ptr<typename T::element_type>> );

template<NonUniquePointer T>
 bool display(T ptr) // function pointer and shared pionter as arguent, but no unique_ptr
{
    std::cout<<typeid(ptr).name()<<std::endl;
    return true;
}
int main()
{
    int *rp = new int(5);
    std::shared_ptr<int> sp = std::make_shared<int>(10);
    std::unique_ptr<int> up = std::make_unique<int>(10);
    display(rp);
    display(sp);
    //display(up); // compilation error
    static_assert( std::is_pointer_v<decltype(rp)> == 1);
    static_assert( std::is_pointer_v<decltype(up)> == 0);
    static_assert( std::is_pointer_v<decltype(sp)> == 0);

    static_assert( rba::is_unique_ptr<decltype(up)>::value == 1);
    static_assert( std::is_pointer_v<decltype(sp)> == 0);

    static_assert( rba::is_smart_ptr<decltype(sp)>::value == 1);
    //static_assert( rba::is_smart_ptr_v<decltype(sp)>  == 1); // does not work, need to check
    //static_assert( rba::is_smart_ptr_v<decltype(rp)>== 0);

    return 0;
}
