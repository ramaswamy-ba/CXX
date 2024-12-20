#include <iostream>
#include <map>
#include <ranges>
int main()
{
    std::map<std::string, int> data
    { 
        { "RAMA" , 80}, { "SWAMY", 90},  {"RAMASWAMY",  95}, {"ABC", 10 }, { "ZAD",  85}, { "ASDF", 88}
    };
    
    for (
            const auto &elm: data 
            | std::views::filter([] (const auto& d) { return d.second > 79; })
            | std::views::take(3) // take onl first 3 elements
            | std::views::keys // return  onl keys
            
        )
        {
            std::cout<<"- "<<elm<<'\n';
        }
        
    return 0;
}
