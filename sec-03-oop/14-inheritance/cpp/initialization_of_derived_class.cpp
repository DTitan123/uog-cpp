class Parent
{
public:
    int id {};

    Parent(int id=0) : id{ id }  {}

    int get_id() const { return id; }
};

class Child: public Parent
{
public:
    double cost {};

    Child(double cost=0.0) : cost{ cost } {}

    double get_cost() const { return cost; }
};

int main()
{   
    // With non-derived classes, constructors only have to worry about
    // their own members.
    Parent parent { 42 };   // use Parent(int) constructor

    // How about the the "id" member that child inherits from Parent?
    Child child { 42.24 };  // use Child(double) constructor

    return 0;
}