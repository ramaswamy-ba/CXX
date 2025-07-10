#include <iostream>
#include <concepts>
#include <vector>
#include <map>

template <typename T1, typename T2>
std::ostream& operator<<( std::ostream &out, const std::pair<T1, T2> &val)
{
    out<<val.fist<<',' <<val.second <<'\n';
    return out;
}

template<typename T>
concept Printable = requires (T val) { std::cout<< val; };

template<std::ranges::input_range Range>
requires Printable<std::ranges::range_value_t<Range>>
void print(const Range &container)
{
    for(const auto& element: container)
        std::cout<< element <<' ';
    std::cout<<'\n';
}

template<std::ranges::input_range OuterRange>
requires (std::ranges::input_range<std::ranges::range_value_t<OuterRange>> && !Printable<std::ranges::range_value_t<OuterRange>> )
void print(const OuterRange &container)
{
    for(const auto& element: container)
        print(element);
}


int main()
{
    std::vector<std::vector<int>> ivec {{1,3,4}, {4,8,9}};
    print(ivec);
    return 0;
}
