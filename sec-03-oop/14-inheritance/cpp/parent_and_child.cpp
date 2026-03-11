class Parent
{
public:
    int parent_value {};

    Parent(int value=0)
        : parent_value { value }
    {
    }

    int get_value() const { return parent_value; }
};

class Child: public Parent
{
public:
    double child_value {};

    Child(double value=0.0)
        : child_value { value }
    {
    }

    double get_subvalue() const { return child_value; }
};