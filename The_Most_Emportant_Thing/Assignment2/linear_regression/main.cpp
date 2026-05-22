// This `main.cpp` does three things:
//  1. Open and read a CSV file (relative path: ../data/concrete.csv).
//  2. Parse each line into numeric features and a target value.
//  3. Fit a LinearRegression model, evaluate with R², and print a prediction demo.

#include "linear_reg.hpp"
#include <fstream>    // std::ifstream
#include <iostream>   // std::cout, std::cerr
#include <sstream>    // std::istringstream
#include <string>     // std::string
#include <vector>     // std::vector
#include <algorithm>  // std::shuffle
#include <random>     // std::mt19937

// train_test_split()
// Shuffles indices with fixed seed 42 (matches assignment's random_state=42)
// then splits into training and test sets based on test_ratio.
void train_test_split(const std::vector<std::vector<double>>& X,
                      const std::vector<double>& y,
                      double test_ratio,
                      std::vector<std::vector<double>>& X_train,
                      std::vector<double>&               y_train,
                      std::vector<std::vector<double>>& X_test,
                      std::vector<double>&               y_test)
{
    const int m { static_cast<int>(X.size()) };
    std::vector<int> indices(m);
    for (int i { 0 }; i < m; ++i) indices[i] = i;
    std::mt19937 rng(42); // fixed seed for reproducibility
    std::shuffle(indices.begin(), indices.end(), rng);

    const int n_test  { static_cast<int>(m * test_ratio) };
    const int n_train { m - n_test };

    for (int i { 0 }; i < n_train; ++i) {
        X_train.push_back(X[indices[i]]);
        y_train.push_back(y[indices[i]]);
    }
    for (int i { n_train }; i < m; ++i) {
        X_test.push_back(X[indices[i]]);
        y_test.push_back(y[indices[i]]);
    }
}

int main() {
    // Open the CSV file using a relative path. Change this path if your CSV
    // is elsewhere. If opening fails we print an error and return non-zero.
    std::ifstream file("../data/concrete.csv");
    if (!file) {
        std::cerr << "Error: could not open ../data/concrete.csv\n";
        return 1; // non-zero indicates an error to the caller
    }

    // `inputs` stores feature vectors (each inner vector is one sample).
    // `outputs` stores a single target value per sample (last column of CSV).
    std::vector<std::vector<double>> inputs;
    std::vector<double> outputs;

    std::string line;

    // Read and discard the first line assuming it's a header row.
    if (!std::getline(file, line)) {
        std::cerr << "Error: file appears to be empty\n";
        return 1;
    }

    // For each remaining line, split on commas and convert tokens to double.
    while (std::getline(file, line)) {
        if (line.empty()) continue; // skip blank lines in the file

        std::istringstream linestream(line);
        std::string cell;
        std::vector<double> row; // numeric values for this CSV row

        // Extract comma-separated tokens. For each token:
        //  - if empty -> use 0.0 (simple, explicit choice for missing data)
        //  - otherwise try stod(); on failure fall back to 0.0
        while (std::getline(linestream, cell, ',')) {
            if (cell.empty()) {
                row.push_back(0.0);
            } else {
                try {
                    row.push_back(std::stod(cell));
                } catch (...) {
                    // Non-numeric fields are treated as 0.0 to keep parsing
                    // simple and robust for this assignment.
                    row.push_back(0.0);
                }
            }
        }

        if (row.empty()) continue; // nothing parsed on this line

        // Convention: the last value in each row is the target (y). We
        // remove it from the feature list and store it separately.
        outputs.push_back(row.back());
        row.pop_back();
        inputs.push_back(row);
    }

    // If no samples were loaded, inform the user and return a non-zero code.
    if (inputs.empty()) {
        std::cout << "No samples loaded\n";
        return 1;
    }

    // Print a short dataset summary so we know parsing succeeded.
    std::cout << "Samples loaded  : " << inputs.size() << "\n";
    std::cout << "Features/sample : " << inputs[0].size() << "\n";

    // Split into 80% train, 20% test — mirrors train_test_split(random_state=42)
    std::vector<std::vector<double>> X_train, X_test;
    std::vector<double>              y_train, y_test;
    train_test_split(inputs, outputs, 0.20, X_train, y_train, X_test, y_test);

    std::cout << "Training samples: " << X_train.size() << "\n";
    std::cout << "Test samples    : " << X_test.size()  << "\n\n";

    // Create and train the LinearRegression model.
    // Mirrors: model = LinearRegression()
    //          model.fit(X_train, y_train)
    sklearn_cpp::linear_model::LinearRegression model;
    model.fit(X_train, y_train, 1e-7, 1000);

    // Print a small demonstration of predictions vs actual targets for the
    // first few samples from the test set.
    std::cout << "Prediction vs Actual (first 5 from test set):\n";
    std::cout << "-----------------------------\n";

    for (int i { 0 }; i < 5 && i < static_cast<int>(X_test.size()); ++i) {
        double pred = model.predict(X_test[i]);
        std::cout << "Sample " << i
                  << " | Predicted: " << pred
                  << " | Actual: " << y_test[i]
                  << "\n";
    }

    // R² score — mirrors sklearn's model.score(X_test, y_test)
    std::cout << "\nR² score (training) : " << model.score(X_train, y_train) << "\n";
    std::cout << "R² score (test)     : " << model.score(X_test,  y_test)  << "\n";

    return 0; // successful run
}