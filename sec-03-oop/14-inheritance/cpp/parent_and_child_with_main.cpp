#include <iostream>
class Parent
{
public:
    int parent_value {};

    Parent(int value=0)
        : parent_value { value }
    { std::cout << "Parent object is constructed!\n"; }

    int get_value() const { return parent_value; }
};

class Child: public Parent
{
public:
    double child_value {};

    Child(double value=0.0)
        : child_value { value }
    { std::cout << "Child object is constructed!\n"; }

    double get_subvalue() const { return child_value; }
};

int main ()
{
    std::cout << "Instantiating a Parent object!\n";
    Parent parent;
    std::cout << "===============================\n";
    std::cout << "Instantiating a Child object!\n";
    Child child;

    return 0;
}