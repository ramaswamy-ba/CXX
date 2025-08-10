#include <functional>
#include <iostream>
#include <memory>
#include <utility>

// Primary template declaration
template<typename Signature>
class Function;

// Specialization for function signatures
template<typename R, typename... Args>
class Function<R(Args...)>
{
    struct CallableBase {
        virtual R call(Args... args) const = 0;
        virtual ~CallableBase() = default;
    };

    template<typename F>
    struct CallableImpl : CallableBase
    {
        F func;
        CallableImpl(F&& f) : func(std::forward<F>(f)) {}
        R call(Args... args) const override { return func(std::forward<Args>(args)...);
    };

    std::unique_ptr<CallableBase> callable;

public:
    Function() = default;

    template<typename F>
    Function(F&& f) : callable(std::make_unique<CallableImpl<F>>(std::forward<F>(f))) {

    R operator()(Args... args) const
    {
        if (!callable)
            throw std::bad_function_call();
        return callable->call(std::forward<Args>(args)...);
    }

    explicit operator bool() const noexcept { return static_cast<bool>(callable); }
};

int main() {
    // Function returning int
    Function<int()> f1 = [] { std::cout << "Hello!\n"; return 42; };
    std::cout << "Returned: " << f1() << "\n";

    // Function returning void
    Function<void(int)> f2 = [](int x) { std::cout << "Got: " << x << "\n"; };
    f2(99);

    // Empty function check
    Function<void()> f3;
    if (!f3) std::cout << "f3 is empty\n";
}
