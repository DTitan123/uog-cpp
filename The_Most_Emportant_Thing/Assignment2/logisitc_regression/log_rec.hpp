// log_rec.hpp
// A self-contained LogisticRegression class that mirrors the
// scikit-learn API used in the assignment:
//
//   model.fit(X_train, y_train, lr, iters)
//   model.predict(sample)           -> int  (0 or 1)
//   model.predict_proba(sample)     -> double in [0,1]
//   model.score(X_test, y_test)     -> double accuracy
//
// Binary classification only (labels 0 and 1).
// Training uses batch gradient descent on the log-loss (cross-
// entropy) cost function, exactly as derived in the assignment.


#pragma once

#include <cmath>     // std::exp, std::log
#include <iostream>  // std::cout
#include <numeric>   // std::inner_product
#include <vector>    // std::vector

namespace sklearn_cpp {
namespace linear_model {

class LogisticRegression {
public:

    // Constructor – weights and bias start at zero; they are
    // resized and initialised properly inside fit().

    LogisticRegression() : bias_(0.0) {}


    // fit()
    //
    // Trains the model using batch gradient descent.
    //
    // Parameters
    //   X            : (m x n) matrix of training features
    //   y            : vector of m binary labels (0 or 1)
    //   learning_rate: gradient-descent step size (e.g. 1e-4)
    //   iterations   : number of full passes over the data
    //
    // The update rule for each step is:
    //   dw_j = (1/m) * sum_i [ (f(x^i) - y^i) * x^i_j ]
    //   db   = (1/m) * sum_i [ (f(x^i) - y^i)          ]
    //   w  <- w - lr * dw
    //   b  <- b - lr * db
    //
    // where f(x^i) = sigmoid(w . x^i + b).

    void fit(const std::vector<std::vector<double>>& X,
             const std::vector<double>& y,
             double learning_rate = 1e-4,
             int    iterations    = 1000)
    {
        const int m = static_cast<int>(X.size());      // number of samples
        const int n = static_cast<int>(X[0].size());   // number of features

        // Initialise weights to zero and bias to zero.
        weights_.assign(n, 0.0);
        bias_ = 0.0;

        // Gradient descent loop.
        for (int iter = 0; iter < iterations; ++iter) {

            // Accumulators for gradients (initialised to zero each iter).
            std::vector<double> dw(n, 0.0);
            double db = 0.0;

            // Accumulate gradients over every training sample.
            for (int i = 0; i < m; ++i) {
                // Forward pass: compute sigmoid(w . x^i + b).
                double z    = dot(weights_, X[i]) + bias_;
                double f_xi = sigmoid(z);

                // Prediction error for sample i.
                double err = f_xi - y[i];

                // Accumulate feature-wise gradients.
                for (int j = 0; j < n; ++j) {
                    dw[j] += err * X[i][j];
                }
                db += err;
            }

            // Average the gradients and update parameters.
            for (int j = 0; j < n; ++j) {
                weights_[j] -= learning_rate * (dw[j] / m);
            }
            bias_ -= learning_rate * (db / m);

            // Print the cost every 100 iterations so the user can see
            // whether training is converging.
            if (iter % 100 == 0) {
                double cost = compute_cost(X, y, m);
                std::cout << "Iteration " << iter
                          << " | Cost: " << cost << "\n";
            }
        }
    }


    // predict_proba()
    //
    // Returns the probability that a single sample belongs to
    // class 1 (i.e. "normal" in the ECG context).

    double predict_proba(const std::vector<double>& x) const {
        return sigmoid(dot(weights_, x) + bias_);
    }


    // predict()
    //
    // Returns the predicted class label (0 or 1).
    // Decision threshold is 0.5 (standard logistic regression).

    int predict(const std::vector<double>& x) const {
        return (predict_proba(x) >= 0.5) ? 1 : 0;
    }


    // score()
    //
    // Computes accuracy on a labelled dataset:
    //   accuracy = (number of correct predictions) / m
    //
    // This mirrors sklearn's model.score(X_test, y_test).

    double score(const std::vector<std::vector<double>>& X,
                 const std::vector<double>& y) const
    {
        int correct = 0;
        for (int i = 0; i < static_cast<int>(X.size()); ++i) {
            if (predict(X[i]) == static_cast<int>(y[i])) {
                ++correct;
            }
        }
        return static_cast<double>(correct) / static_cast<double>(X.size());
    }

private:
    std::vector<double> weights_; // learned weight vector (size n)
    double              bias_;    // learned bias term


    // sigmoid() – the logistic function: 1 / (1 + e^(-z))
    // Clamps z to avoid overflow in std::exp for very large |z|.

    static double sigmoid(double z) {
        // Clamp to a safe range to prevent exp overflow.
        if (z >  500.0) return 1.0;
        if (z < -500.0) return 0.0;
        return 1.0 / (1.0 + std::exp(-z));
    }

    // dot() – dot product of two equal-length vectors.

    static double dot(const std::vector<double>& a,
                      const std::vector<double>& b)
    {
        double result = 0.0;
        for (int i = 0; i < static_cast<int>(a.size()); ++i) {
            result += a[i] * b[i];
        }
        return result;
    }


    // compute_cost() – binary cross-entropy loss.
    // Used only for the per-iteration progress printout.
    //
    //   J = -(1/m) * sum_i [ y^i * log(f) + (1-y^i) * log(1-f) ]

    double compute_cost(const std::vector<std::vector<double>>& X,
                        const std::vector<double>& y,
                        int m) const
    {
        const double eps = 1e-15; // avoid log(0)
        double cost = 0.0;
        for (int i = 0; i < m; ++i) {
            double f = predict_proba(X[i]);
            // Clamp f away from 0 and 1 to keep log finite.
            f = std::max(eps, std::min(1.0 - eps, f));
            cost += y[i] * std::log(f) + (1.0 - y[i]) * std::log(1.0 - f);
        }
        return -cost / m;
    }
};

} // namespace linear_model
} // namespace sklearn_cpp
