#include <iostream>
#include <vector>

template <typename T>
class Base {
public:
    virtual void Go() = 0;
    virtual T Get() const = 0;
    virtual ~Base() = default;
};

template <typename T>
class A : public Base<T> {
public:
    A() {}

    void Go() override {
        std::cout << "A" << std::endl;
    }

    T Get() const override {
        std::cout << "A" << std::endl;
        return t;
    }

private:
    T t{};
};

template <typename T>
class B : public Base<T> {
public:
    B() {}

    void Go() override {
        std::cout << "B" << std::endl;
    }

    T Get() const override {
        std::cout << "B" << std::endl;
        return t;
    }

private:
    T t{};
};

int main() {
    std::vector<Base<double>*> vec;

    A<double>* a = new A<double>();
    B<int64_t>* b = new B<int64_t>();

    vec.push_back(a);
    // vec.push_back(b);

    for (auto& iter : vec) {
        iter->Go();
        double result = iter->Get(); // Use the correct type for Get()
        std::cout << "Result: " << result << std::endl;
    }

    // Don't forget to delete allocated objects
    delete a;
    delete b;

    return 0;
}
