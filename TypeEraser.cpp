#include <iostream>
#include <memory>
#include <string>
#include <vector>

//std::function and std::any are implemented with similar logic
struct TypeEraser
{
    struct TypeKeeperBase
    {
        virtual std::string getName() const = 0;
        virtual ~TypeKeeperBase() {}

    };
    template <class T>
    struct TypeKeeper : public TypeKeeperBase
    {
        T object;
        TypeKeeper(const T& obj) : object(obj) { }
        std::string getName() const override { return object.getName(); } // every T type should have getName method implemented
    };
private:
    std::shared_ptr<TypeKeeperBase> object;
public:
    template<class T>
    TypeEraser(T&& obj) : object(std::make_shared<TypeKeeper<T>>(std::forward<T>(obj))) { }
    std::string getName() const { return object->getName(); }
};

struct Mango { std::string getName() const { return "Mango"; } };
struct Apple { std::string getName() const { return "Apple"; } };

void TestTypeEraser()
{
    std::vector<TypeEraser> fruits{TypeEraser(Mango()), TypeEraser(Apple())};
    for (auto f : fruits)
    {
        std::cout << f.getName() << '\n';
    }

}
int main()
{
	std::cout << "TypeEraser Demon";
    TestTypeEraser();
	return 0;
}
