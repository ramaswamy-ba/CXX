#include <iostream>
template <typename...> 
struct Tuple {};

template <typename First, typename... Rest>
struct Tuple<First, Rest...>: public Tuple<Rest...>
{
    First first;
    Tuple(First f, Rest... r) : Tuple<Rest...>(std::forward<Rest>(r)...), first(std::forward<First>(f)) { }
};

 template<size_t N, typename First, typename... Rest>
 auto& get(Tuple<First, Rest...>& t)
 {
     static_assert(N >= 0, "Wrong Index -ve");
     static_assert(N <= sizeof...(Rest)+1, "Wrong Index,  N >  tuple args");
     std::cout<<"Size of "<<sizeof...(Rest)+1<<std::endl;
     if constexpr (N == 0)
         return t.first;
     else
         return get<N - 1>(static_cast<Tuple<Rest...>&>(t));
 }

 void TupleTest()
 {
     Tuple<int, std::string> t(1, "RAMA");
     auto res = get<1>(t);
     std::cout << res;

 }

int main() 
{
    TupleTest();
    return 0;
}
