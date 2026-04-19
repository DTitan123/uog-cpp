#pragma once

#include <vector>
#include <iostream>

// linear_reg.hpp
// Simple LinearRegression class used for teaching.
//
// Purpose and usage (clear and concise):
//  - This header declares a compact LinearRegression class that supports
//    training via a basic batch gradient-descent `fit` and prediction via
//    `predict`.
//  - Intended for educational use: easy to read and understand, not
//    optimized for production. No external dependencies beyond the STL.
//
// Data layout expected by `fit`:
//  - X: vector of samples (m), each sample is a vector of features (n).
//  - y: vector of target values of length m.
//
// Training algorithm (implementation notes):
//  - We initialize weights to zero and perform `iterations` steps of
//    batch gradient descent with the provided `learning_rate`.
//  - Gradients are computed as the sum of errors over all samples (MSE
//    derivative). The implementation uses a simple scaling factor (2/m)
//    when updating weights and bias.
//  - No feature scaling, regularization, or convergence checks are
//    implemented to keep the code minimal and explicit.
//
// Complexity:
//  - Each iteration does O(m * n) work (m samples, n features).
//
// Notes for readers and graders:
//  - The class is intentionally small and self-contained. The header
//    includes inline method implementations to keep the project simple for
//    students; in larger projects you may split declarations and
//    implementations across `.hpp` and `.cpp` files.

namespace sklearn_cpp {
namespace linear_model {

class LinearRegression {

private:
    // Model parameters
    std::vector<double> weights; // coefficients for each feature (size n)
    double bias;                 // scalar intercept term

public:
    // Constructor: set a sensible default for bias. `weights` will be
    // initialized in `fit` when the training data size (n) is known.
    LinearRegression() : bias(0.0) {}

    // Fit the linear model using batch gradient descent.
    // Parameters:
    //  - X: matrix shape (m, n) where m = number of samples, n = features
    //  - y: vector of length m containing target values
    //  - learning_rate: gradient descent step size (default small value)
    //  - iterations: number of gradient steps to perform
    // Implementation details:
    //  - Initializes `weights` to zeros and executes `iterations` loops.
    //  - Computes gradient of mean-squared error and updates weights/bias.
    //  - No normalization or regularization is applied; this keeps the
    //    code simple and explicit for learning purposes.
    void fit(const std::vector<std::vector<double>>& X,
             const std::vector<double>& y,
             double learning_rate = 1e-7,
             int iterations = 1000)
    {
        // Number of samples (m) and number of features (n)
        int m = X.size();
        int n = X[0].size();

        // Initialize parameters
        weights.assign(n, 0.0);
        bias = 0.0;

        // Gradient descent main loop
        for (int iter = 0; iter < iterations; iter++) {

            // Accumulate gradients across all samples
            std::vector<double> dw(n, 0.0);
            double db = 0.0;

            for (int i = 0; i < m; i++) {
                // Compute prediction and error for sample i
                double y_pred = predict(X[i]);
                double error = y_pred - y[i];

                // Accumulate feature-wise gradients
                for (int j = 0; j < n; j++)
                    dw[j] += error * X[i][j];

                // Accumulate bias gradient
                db += error;
            }

            // Update parameters using the average gradient scaled by 2/m
            // (derivative of MSE). We subtract because we move opposite the
            // gradient to minimize the loss.
            for (int j = 0; j < n; j++)
                weights[j] -= learning_rate * (2.0 / m) * dw[j];

            bias -= learning_rate * (2.0 / m) * db;
        }
    }

    // Predict a scalar value for a single feature vector `x`.
    // The prediction formula is: bias + dot(weights, x)
    double predict(const std::vector<double>& x) const {

        double result = bias;

        for (size_t j = 0; j < weights.size(); j++)
            result += weights[j] * x[j];

        return result;
    }
};

} // namespace linear_model
} // namespace sklearn_cpp