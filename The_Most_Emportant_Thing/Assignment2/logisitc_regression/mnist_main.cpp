// mnist_main.cpp

// Handwritten Digit Classification using LogisticRegression
//
// This program does three things, matching the assignment spec:
//  1. Opens and reads mnist_micro.csv (3000 samples, 784 features + 1 label)
//  2. Trains 10 binary LogisticRegression classifiers using one-vs-rest:
//     each classifier learns to distinguish one digit from all others
//  3. Predicts the digit by picking the class with the highest probability
//     and reports accuracy along with a 6-sample prediction demo
//
// Label convention (from the dataset):
//   0-9 representing which digit the image shows


#include "logistic_regression.hpp"  // LogisticRegression class

#include <algorithm>  // std::max_element
#include <fstream>    // std::ifstream
#include <iostream>   // std::cout and std::cerr
#include <sstream>    // std::istringstream
#include <string>     // std::string
#include <vector>     // std::vector



// load_csv()
//
// Opens the CSV at `path` and reads every row
// The last column is treated as the label y (digit 0-9)
// all preceding columns are features X
//
// Returns true on success and false on any I/O error

bool load_csv(const std::string& path,
              std::vector<std::vector<double>>& X,
              std::vector<double>& y)
{
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Error: could not open " << path << "\n";
        return false;
    }

    std::string line;

    // mnist_micro.csv has no header row so we read straight away
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream ss(line);
        std::string cell;
        std::vector<double> row;

        // parse comma-separated values; fall back to 0.0 on failure
        while (std::getline(ss, cell, ',')) {
            if (cell.empty()) {
                row.push_back(0.0);
            } else {
                try {
                    row.push_back(std::stod(cell));
                } catch (...) {
                    row.push_back(0.0);
                }
            }
        }

        if (row.empty()) continue;

        // convention: last column = digit label y
        y.push_back(row.back());
        row.pop_back();
        X.push_back(row);
    }

    return !X.empty();
}


int main() {

    // 1. Load the MNIST micro dataset from CSV

    std::vector<std::vector<double>> X;   // all feature vectors
    std::vector<double>              y;   // all labels (0-9)

    if (!load_csv("../data/mnist_micro.csv", X, y)) {
        return 1;
    }

    std::cout << "=== Handwritten Digit Classification ===\n\n";
    std::cout << "Samples loaded  : " << X.size()    << "\n";
    std::cout << "Features/sample : " << X[0].size() << "\n\n";

    // count class distribution
    std::cout << "Samples per digit:\n";
    for (int d { 0 }; d < 10; ++d) {
        int count { 0 };
        for (double label : y)
            if (static_cast<int>(label) == d) count++;
        std::cout << "  Digit " << d << ": " << count << "\n";
    }
    std::cout << "\n";



    // 2. Train 10 binary classifiers using one-vs-rest
    //
    //    For each digit d we create a binary label vector:
    //      1 if the sample is digit d and 0 otherwise
    //    We then train a LogisticRegression on this binary problem
    //    Doing this for all 10 digits gives us 10 classifiers
    //
    //    This mirrors the approach used in sklearn's LogisticRegression
    //    when applied to a multi-class problem

    const int n_classes { 10 };
    std::vector<sklearn_cpp::linear_model::LogisticRegression> classifiers;
    classifiers.reserve(n_classes);

    std::cout << "--- Training ---\n";

    for (int digit { 0 }; digit < n_classes; ++digit) {

        // build binary label: 1 if this sample is `digit` and 0 otherwise
        std::vector<double> y_binary(y.size());
        for (int i { 0 }; i < static_cast<int>(y.size()); ++i) {
            y_binary[i] = (static_cast<int>(y[i]) == digit) ? 1.0 : 0.0;
        }

        // train a logistic regression for this digit
        sklearn_cpp::linear_model::LogisticRegression clf;
        clf.fit(X, y_binary, 1e-4, 500);
        classifiers.push_back(clf);

        std::cout << "  Digit " << digit << " trained\n";
    }



    // 3. Predict: for each sample run all 10 classifiers and pick
    //    the digit with the highest predicted probability
    //    This mirrors: predicted = argmax(proba) in Python

    int correct { 0 };
    const int m { static_cast<int>(X.size()) };

    for (int i { 0 }; i < m; ++i) {
        // get probability from each classifier
        std::vector<double> probs(n_classes);
        for (int d { 0 }; d < n_classes; ++d) {
            probs[d] = classifiers[d].predict_proba(X[i]);
        }

        // predicted digit = index of highest probability
        int predicted { static_cast<int>(
            std::max_element(probs.begin(), probs.end()) - probs.begin()
        )};

        if (predicted == static_cast<int>(y[i])) {
            correct++;
        }
    }

    double accuracy { static_cast<double>(correct) / m };

    std::cout << "\n--- Accuracy ---\n";
    std::cout << "Training set accuracy : " << accuracy << "\n";



    // 4. Visualise predictions on 6 samples
    //    Mirrors the demo in the assignment brief

    std::cout << "\n--- Sample Predictions (first 6) ---\n";
    std::cout << "Sample | Predicted | Actual\n";
    std::cout << "-------|-----------|-------\n";

    for (int i { 0 }; i < 6 && i < m; ++i) {
        std::vector<double> probs(n_classes);
        for (int d { 0 }; d < n_classes; ++d) {
            probs[d] = classifiers[d].predict_proba(X[i]);
        }
        int predicted { static_cast<int>(
            std::max_element(probs.begin(), probs.end()) - probs.begin()
        )};
        std::cout << "  " << i << "    | "
                  << predicted << "         | "
                  << static_cast<int>(y[i]) << "\n";
    }

    return 0; // success
}