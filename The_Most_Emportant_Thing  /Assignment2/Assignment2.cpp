#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ifstream file("data/concrete.csv");

    std::string line;

    while (getline(file, line))
    {
        std::cout << line << std::endl;
    }
}
namespace sklearn_cpp
{
    namespace linear_model
    {
        class LinearRegression
        {

            fit(x, y)
            predict(x)
        };

        class LogisticRegression
        {
            fit()
            predict()
        };
    }
}