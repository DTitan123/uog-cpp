// No report is required for this assignment. The goal is to comment the
// implementation clearly and concisely so a reader understands how the
// CSV is read and how the linear regression is used.
//
// This `main.cpp` does three things:
//  1. Open and read a CSV file (relative path: ../data/concrete.csv).
//  2. Parse each line into numeric features and a target value.
//  3. Fit a simple LinearRegression model and print a small prediction demo.

#include "linear_reg.hpp"
#include <fstream>   // std::ifstream
#include <iostream>  // std::cout, std::cerr
#include <sstream>   // std::istringstream
#include <string>    // std::string
#include <vector>    // std::vector

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

    // Read and discard the first line assuming it's a header row. If your CSV
    // has no header, remove the getline below so the first data row is read.
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
    std::cout << "Samples loaded: " << inputs.size() << "\n";
    std::cout << "Features per sample: " << inputs[0].size() << "\n";

    // Create and use the simple LinearRegression class defined in
    // `linear_reg.hpp`. The model stores weights and a bias and provides
    // `fit` (gradient-descent training) and `predict` (single-sample).
    sklearn_cpp::linear_model::LinearRegression model;

    // Train the model. Parameters:
    //  - inputs: matrix of shape (m, n)
    //  - outputs: vector of length m
    //  - learning_rate: step size for gradient descent
    //  - iterations: number of training steps
    // These hyperparameters are small/simple here to keep training stable.
    model.fit(inputs, outputs, 1e-7, 1000);

    // Print a small demonstration of predictions vs actual targets for the
    // first few samples. This shows the model was trained and can produce
    // numeric outputs; it is not a thorough evaluation.
    std::cout << "\nPrediction vs Actual:\n";
    std::cout << "-----------------------------\n";

    for (int i = 0; i < 5 && i < (int)inputs.size(); i++) {
        double pred = model.predict(inputs[i]);
        std::cout << "Sample " << i
                  << " | Predicted: " << pred
                  << " | Actual: " << outputs[i]
                  << "\n";
    }

    // Successful run
    return 0;
}
