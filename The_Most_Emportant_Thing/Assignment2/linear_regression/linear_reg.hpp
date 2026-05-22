// linear_reg.hpp
// Simple LinearRegression class for ENG3091 Assignment 02.
//
// co-pilot helped me write this code <3 thx co-pilot
// Purpose:
//  - Implements a LinearRegression class that trains via batch gradient
//    descent and predicts via a dot product of weights and features.
//  - No external dependencies beyond the STL.
//
// Data layout expected by fit():
//  - X: vector of samples (m), each sample is a vector of features (n).
//  - y: vector of target values of length m.
//
// Training algorithm:
//  - Weights are initialised to zero and updated over `iterations` steps
//    of batch gradient descent with the provided `learning_rate`.
//  - Gradients are computed as the sum of errors over all samples (MSE
//    derivative), scaled by 2/m.
//  - No feature scaling or regularisation is applied.
//
// Complexity:
//  - Each iteration does O(m * n) work (m samples, n features).
#pragma once

#include <vector>

namespace sklearn_cpp {
namespace linear_model {

// BaseModel
//
// Abstract base class shared by LinearRegression and LogisticRegression.
// Both models expose the same fit() and predict() interface, so we define
// it once here and inherit in each subclass.
class BaseModel {
public:
    // fit() and predict_value() are pure virtual: every subclass must implement them.
    virtual void fit(const std::vector<std::vector<double>>& X,
                     const std::vector<double>& y,
                     double learning_rate,
                     int iterations) = 0;

    virtual double predict_value(const std::vector<double>& x) const = 0;

    // Virtual destructor: required whenever a class has virtual functions.
    virtual ~BaseModel() = default;
};

class LinearRegression : public BaseModel {

private:
    // Model parameters
    std::vector<double> weights; // coefficients for each feature (size n)
    double bias;                 // scalar intercept term

public:
    // Constructor: initialises bias to zero. weights are initialised
    // in fit() once the number of features is known.
    LinearRegression() : bias { 0.0 } {}

    // fit() — trains the model using batch gradient descent.
    // Parameters:
    //  - X: matrix of shape (m, n), m samples and n features
    //  - y: vector of length m containing target values
    //  - learning_rate: step size for gradient descent
    //  - iterations: number of gradient steps to perform
    // Initialises weights to zero and updates them each iteration
    // using the MSE gradient scaled by 2/m. No normalisation or
    // regularisation is applied.
    void fit(const std::vector<std::vector<double>>& X,
             const std::vector<double>& y,
             double learning_rate = 1e-7,
             int iterations = 1000) override
    {
        // number of samples (m) and number of features (n)
        const int m { static_cast<int>(X.size()) };
        const int n { static_cast<int>(X[0].size()) };

        // initialise parameters
        weights.assign(n, 0.0);
        bias = 0.0;

        // gradient descent main loop
        for (int iter = 0; iter < iterations; iter++) {

            // accumulate gradients across all samples
            std::vector<double> dw(n, 0.0);
            double db = 0.0;

            for (int i = 0; i < m; i++) {
                // compute prediction and error for sample i
                double y_pred = predict(X[i]);
                double error = y_pred - y[i];

                // accumulate feature-wise gradients
                for (int j = 0; j < n; j++)
                    dw[j] += error * X[i][j];

                // accumulate bias gradient
                db += error;
            }

            // update parameters using the average gradient scaled by 2/m
            // (derivative of MSE). subtract because we move opposite the
            // gradient to minimise the loss.
            for (int j = 0; j < n; j++)
                weights[j] -= learning_rate * (2.0 / m) * dw[j];

            bias -= learning_rate * (2.0 / m) * db;
        }
    }

    // predict() — returns a scalar prediction for a single feature vector.
    // Formula: bias + dot(weights, x)
    double predict(const std::vector<double>& x) const {
        double result = bias;
        for (size_t j = 0; j < weights.size(); j++)
            result += weights[j] * x[j];
        return result;
    }

    // predict_value() satisfies the BaseModel interface.
    // Delegates to predict() so existing code is unchanged.
    double predict_value(const std::vector<double>& x) const override {
        return predict(x);
    }

    // score() — returns R² (coefficient of determination).
    // R² = 1 - SS_res / SS_tot
    // 1.0 = perfect fit, 0.0 = no better than predicting the mean.
    // Mirrors sklearn's LinearRegression.score(X, y).
    double score(const std::vector<std::vector<double>>& X,
                 const std::vector<double>& y) const
    {
        const int m { static_cast<int>(X.size()) };

        double y_mean { 0.0 };
        for (int i { 0 }; i < m; ++i)
            y_mean += y[i];
        y_mean /= m;

        double ss_res { 0.0 };
        double ss_tot { 0.0 };
        for (int i { 0 }; i < m; ++i) {
            double error { predict(X[i]) - y[i] };
            ss_res += error * error;
            ss_tot += (y[i] - y_mean) * (y[i] - y_mean);
        }

        if (ss_tot == 0.0) return 1.0;
        return 1.0 - (ss_res / ss_tot);
    }
};

} // namespace linear_model
} // namespace sklearn_cpp