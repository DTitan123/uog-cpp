class Parent
{
public:
    int parent_value {};

    Parent(int id=0)
        : parent_value { id }
    {
    }

    int get_value() const { return parent_value; }
};

class Child: public Parent
{
public:
    double child_value {};

    Child(double cost=0.0)
        : child_value { cost }
    {
    }

    double get_subvalue() const { return child_value; }
};