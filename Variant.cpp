#include <algorithm>
#include <type_traits>
#include <utility>
#include <iostream>
#include <variant>

template<typename... Types>
class Variant
{
    static constexpr std::size_t data_size = std::max({sizeof(Types)...});
    static constexpr size_t data_align = std::max({alignof(Types)...});

    //creates a raw memory buffer large and aligned enough to store any of the types in the variant.
    //It's the backbone of type-erased storage
    using Storage = std::aligned_storage_t<data_size, data_align>;
    Storage data;
    size_t index; //is used to record which type is currently active.

    // Use a helper to destroy the current value
    void destroy() { destroy_helper(index); }

    template<size_t I = 0>
    void destroy_helper(size_t i)
    {
        if constexpr (I < sizeof...(Types))
        {
            if (i == I) {
                using T = typename std::tuple_element<I, std::tuple<Types...>>::type;
                reinterpret_cast<T*>(&data)->~T();
            } else
                destroy_helper<I + 1>(i);
        }
    }

public:
    Variant() // to support std::monostate
    {
        index = get_index<std::monostate>();
        new (&data) std::monostate{};
    }

    template<typename T, typename = std::enable_if_t<(std::is_same_v<T, Types> || ...)>>
    Variant(T&& value)
    {
        using U = std::decay_t<T>;
        index = get_index<U>();
        //When a value is assigned, it’s constructed in-place using placement new:
        new (&data) U(std::forward<T>(value));
    }

    ~Variant() { destroy(); }

    template<typename T>
    T& get()
    {
        if (index != get_index<T>())
            throw std::bad_cast();
        return *reinterpret_cast<T*>(&data);
    }

    template<typename T>
    bool holds_alternative() const { return index == get_index<T>(); }

    template<typename T>
    static constexpr size_t get_index() { return get_index_impl<T, 0, Types...>(); }

    template<typename Visitor, size_t I = 0>
    decltype(auto) visit_helper(Visitor&& vis, size_t i)
    {
        if constexpr (I < sizeof...(Types)) {
            if (i == I) {
                using T = typename std::tuple_element<I, std::tuple<Types...>>::type;
                return std::forward<Visitor>(vis)(*reinterpret_cast<T*>(&data));
            } else {
                return visit_helper<Visitor, I + 1>(std::forward<Visitor>(vis), i);
            }
        }
        throw std::bad_cast(); // fallback if index is invalid
    }

    template<typename Visitor>
    decltype(auto) visit(Visitor&& vis) { return visit_helper(std::forward<Visitor>(vis), index); }

private:
    template<typename T, size_t I, typename First, typename... Rest>
    static constexpr size_t get_index_impl()
    {
        if constexpr (std::is_same_v<T, First>) {
            return I;
        } else {
            return get_index_impl<T, I + 1, Rest...>();
        }
    }
};

// to print monostate
std::ostream& operator<<(std::ostream& os, const std::monostate&) { return os << "[monostate]"; }

int main()
{
    Variant<int, std::string, std::monostate> v;
    v = std::string("Hello");

    if (v.holds_alternative<std::string>()) {
        std::cout << "String: " << v.get<std::string>() << "\n";
    }

    v.visit([](auto&& val) { std::cout << "Visited value: " << val << "\n"; });
}
