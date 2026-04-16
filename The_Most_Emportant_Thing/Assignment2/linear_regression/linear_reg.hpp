#pragma once
#include <vector>
#include <iostream>

namespace sklearn_cpp {
namespace linear_model {

class LinearRegression {

private:
    std::vector<double> weights;
    double bias;

public:
    LinearRegression() : bias(0.0) {}

    void fit(const std::vector<std::vector<double>>& X,
             const std::vector<double>& y,
             double learning_rate = 1e-7,
             int iterations = 1000)
    {
        int m = X.size();
        int n = X[0].size();

        weights.assign(n, 0.0);
        bias = 0.0;

        for (int iter = 0; iter < iterations; iter++) {

            std::vector<double> dw(n, 0.0);
            double db = 0.0;

            for (int i = 0; i < m; i++) {

                double y_pred = predict(X[i]);
                double error = y_pred - y[i];

                for (int j = 0; j < n; j++)
                    dw[j] += error * X[i][j];

                db += error;
            }

            for (int j = 0; j < n; j++)
                weights[j] -= learning_rate * (2.0 / m) * dw[j];

            bias -= learning_rate * (2.0 / m) * db;
        }
    }

    double predict(const std::vector<double>& x) const {

        double result = bias;

        for (size_t j = 0; j < weights.size(); j++)
            result += weights[j] * x[j];

        return result;
    }
};

} // namespace linear_model
} // namespace sklearn_cpp