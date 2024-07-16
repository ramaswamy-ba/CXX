#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

constexpr double epsilon_value = 1e-6;
//https://stackoverflow.com/questions/17333/how-do-you-compare-float-and-double-while-accounting-for-precision-loss

bool are_equal(double a, double b, double epsilon= epsilon_value)   { return std::fabs(a - b) < epsilon; }
bool less_than(double a, double b, double epsilon=epsilon_value)    { return (a - b) < epsilon; }
bool greater_than(double a, double b, double epsilon=epsilon_value) { return (a - b) > epsilon; }

class TickTable
{
    std::vector<std::pair<double, double> > tick_interval_table;
    bool valid = false;
    bool is_vald() { return valid;}
public:
    void set_up_ticks()
    {
        tick_interval_table.emplace_back(0.5, 0.01);
        tick_interval_table.emplace_back(1.01, 0.05);
        tick_interval_table.emplace_back(5.01, 0.1);
        tick_interval_table.emplace_back(99.01, 1);
        tick_interval_table.emplace_back(100.01, 1);
        tick_interval_table.emplace_back(500.01, 1);
        tick_interval_table.emplace_back(1000.01,2);
        tick_interval_table.emplace_back(2000.01, 5);

    }
    double getTickPrice(double price)
    {
        if ( are_equal(price, 0) )
            return -1;
        auto ite = std::upper_bound(tick_interval_table.begin(), tick_interval_table.end(), price, [](auto price, auto step) { return less_than(price, step.first); });

        if( ite == tick_interval_table.end() )
        {
            auto ite = tick_interval_table.rbegin();
            std::cout<<"Tick for price "<<price<<"  "<<ite->first<<"->"<<ite->second<<'\n';
            return ite->second;
        }

        if ( ite != tick_interval_table.end())
        {
            --ite;
            std::cout<<"Tick for price "<<price<<"  "<<ite->first<<"->"<<ite->second<<'\n';
            return ite->second;
        }
        return -1;
    }
    bool check()
    {
        if(tick_interval_table.empty())
        {
            std::cerr<<"Tick Table not setup\n";
            valid = false;
        }
        else if ( !std::is_sorted(tick_interval_table.begin(), tick_interval_table.end()) )
        {
            std::cerr<<"Tick table not sorted\n";
            valid = false;
        }
        std::cout<<"Tick table setup is GOOD\n";
        return valid;
    };
};

int main()
{
    TickTable tt;
    tt.set_up_ticks();
    tt.check();
    tt.getTickPrice(208);
    tt.getTickPrice(500);
    tt.getTickPrice(500.01);
    tt.getTickPrice(2000.01);
    tt.getTickPrice(3000);
    return 0;
}
