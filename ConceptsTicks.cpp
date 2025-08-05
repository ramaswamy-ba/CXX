#include <memory>
#include <iostream>
#include <type_traits>
template <class T>
concept SmartPointerType = std::is_same_v<T, std::unique_ptr<typename T::element_type>> || 
                               std::is_same_v<T, std::shared_ptr<typename T::element_type>>;

/*namespace rba
{
  // this tick does not work due to 
  //  - In C++, you can’t have two templates with the same name but different constraints.
    template<class T>
    struct is_smart_ptr : std::false_type {};

    template <SmartPointerType T>
    struct is_smart_ptr : std::true_type {};
}*/

namespace rba
{   
   // Base template for is_smart_ptr, defaults to false
   template<class T, typename = void>
    struct is_smart_ptr : std::false_type {};

    // Specialization of is_smart_ptr for types that satisfy the SmartPointerType concept
    // For these types, is_smart_ptr<T>::value will be true
    
    //template <SmartPointerType T>
    //struct is_smart_ptr : std::true_type {};

    template <typename T>
    struct is_smart_ptr<T, std::enable_if_t<SmartPointerType<T>>> : std::true_type {};
}

//Check for member funtion to exist, which takes args and returns nothing.
template<typename T>
concept HasCallWithArgs = requires(T t, int x, double y) {
    { t.call(x, y) } -> std::convertible_to<void>;
};

void test1()
{
    std::cout << std::boolalpha;
    std::cout << "is_smart_ptr<int>: " << rba::is_smart_ptr<int>::value << '\n';  // Outputs: false
    std::cout << "is_smart_ptr<std::unique_ptr<int>>: " << rba::is_smart_ptr<std::unique_ptr<int>>::value << '\n';  // Outputs: true
    return 0;
}
