// log_main.cpp

// ECG Anomaly Detection using LogisticRegression
//
// This program does three things, matching the assignment spec:
//  1. Opens and reads ecg.csv (4997 samples, 140 features + 1 label).
//  2. Splits the dataset into an 80% training set and 20% test set
//     (mirrors the sklearn train_test_split used in the assignment).
//  3. Trains a LogisticRegression model and reports:
//       - Training accuracy
//       - Test accuracy
//       - A 6-sample prediction demo (actual label vs predicted label)
//
// Label convention (from the assignment):
//   0 = abnormal ECG  (patient is not fine)
//   1 = normal  ECG   (patient is fine)


#include "log_rec.hpp"  // LogisticRegression class

#include <algorithm>  // std::shuffle
#include <fstream>    // std::ifstream
#include <iostream>   // std::cout, std::cerr
#include <random>     // std::mt19937, std::random_device
#include <sstream>    // std::istringstream
#include <string>     // std::string
#include <vector>     // std::vector



// load_csv()
//
// Opens the CSV at `path`, skips one header row, then reads
// every remaining row. The last column is treated as the
// binary label y; all preceding columns are features X.
//
// Returns true on success, false on any I/O error.

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

    // Skip header row (column names).
    if (!std::getline(file, line)) {
        std::cerr << "Error: file appears to be empty\n";
        return false;
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream ss(line);
        std::string cell;
        std::vector<double> row;

        // Parse comma-separated values; fall back to 0.0 on failure.
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

        // Convention: last column = label y.
        y.push_back(row.back());
        row.pop_back();
        X.push_back(row);
    }

    return !X.empty();
}



// train_test_split()
//
// Shuffles indices with a fixed seed (42, matching the
// assignment's random_state=42) then splits the dataset so
// that `test_ratio` of samples go to the test set and the
// remainder go to the training set.

void train_test_split(const std::vector<std::vector<double>>& X,
                      const std::vector<double>& y,
                      double test_ratio,
                      std::vector<std::vector<double>>& X_train,
                      std::vector<double>&               y_train,
                      std::vector<std::vector<double>>& X_test,
                      std::vector<double>&               y_test)
{
    const int m = static_cast<int>(X.size());

    // Build a shuffled index array using random_state = 42.
    std::vector<int> indices(m);
    for (int i = 0; i < m; ++i) indices[i] = i;
    std::mt19937 rng(42); // fixed seed for reproducibility
    std::shuffle(indices.begin(), indices.end(), rng);

    // The first (1 - test_ratio) fraction go to training.
    int n_test  = static_cast<int>(m * test_ratio);
    int n_train = m - n_test;

    X_train.reserve(n_train);  y_train.reserve(n_train);
    X_test.reserve(n_test);    y_test.reserve(n_test);

    for (int i = 0; i < n_train; ++i) {
        X_train.push_back(X[indices[i]]);
        y_train.push_back(y[indices[i]]);
    }
    for (int i = n_train; i < m; ++i) {
        X_test.push_back(X[indices[i]]);
        y_test.push_back(y[indices[i]]);
    }
}


int main() {


    // 1. Load the ECG dataset from CSV.

    std::vector<std::vector<double>> X;   // all feature vectors
    std::vector<double>              y;   // all labels (0 or 1)

    if (!load_csv("../data/ecg.csv", X, y)) {
        return 1;
    }

    std::cout << "=== ECG Anomaly Detection ===\n\n";
    std::cout << "Samples loaded  : " << X.size()    << "\n";
    std::cout << "Features/sample : " << X[0].size() << "\n";

    // Count class distribution (matching the assignment printout).
    int n_abnormal = 0, n_normal = 0;
    for (double label : y) {
        if (label == 0.0) ++n_abnormal;
        else              ++n_normal;
    }
    std::cout << "Labels = 0 (abnormal): " << n_abnormal << "\n";
    std::cout << "Labels = 1 (normal)  : " << n_normal   << "\n\n";



    // 2. Split into 80% training set and 20% test set.
    //    This mirrors: train_test_split(X, y, test_size=0.20,
    //                                  random_state=42)

    std::vector<std::vector<double>> X_train, X_test;
    std::vector<double>              y_train, y_test;

    train_test_split(X, y, 0.20, X_train, y_train, X_test, y_test);

    std::cout << "Training samples: " << X_train.size()
              << "  (" << (100.0 * X_train.size() / X.size()) << "%)\n";
    std::cout << "Test samples    : " << X_test.size()
              << "  (" << (100.0 * X_test.size()  / X.size()) << "%)\n\n";



    // 3. Create and train the LogisticRegression model.
    //
    //    Hyperparameters:
    //      learning_rate = 1e-4  (stable for this dataset)
    //      iterations    = 1000  (enough for good convergence)
    //
    //    This mirrors:
    //      log_reg = LogisticRegression()
    //      log_reg.fit(X_train, y_train)

    std::cout << "--- Training ---\n";
    sklearn_cpp::linear_model::LogisticRegression log_reg;
    log_reg.fit(X_train, y_train, 1e-4, 1000);



    // 4. Evaluate the model.
    //
    //    accuracy_score(y_test, y_pred) in sklearn is reproduced
    //    by model.score(), which counts correct predictions / m.

    double train_accuracy = log_reg.score(X_train, y_train);
    double test_accuracy  = log_reg.score(X_test,  y_test);

    std::cout << "\n--- Accuracy ---\n";
    std::cout << "Training set accuracy : " << train_accuracy << "\n";
    std::cout << "Test set accuracy     : " << test_accuracy  << "\n";



    // 5. Visualise predictions on 6 test samples.
    //    Mirrors the 2x3 subplot demo in the assignment:
    //      for i in range(nrows * ncols):
    //          predicted_label = int(log_reg.predict(data_point))
    //          plt.title(f"Actual: {actual} | Pred: {predicted}")
  
    std::cout << "\n--- Sample Predictions (first 6 from test set) ---\n";
    std::cout << "Sample | P(normal) | Predicted | Actual\n";
    std::cout << "-------|-----------|-----------|-------\n";

    for (int i = 0; i < 6 && i < static_cast<int>(X_test.size()); ++i) {
        double prob      = log_reg.predict_proba(X_test[i]);
        int    predicted = log_reg.predict(X_test[i]);
        int    actual    = static_cast<int>(y_test[i]);

        std::cout << "  " << i << "    | "
                  << prob
                  << " | "
                  << (predicted == 1 ? "normal  " : "abnormal")
                  << " | "
                  << (actual    == 1 ? "normal"   : "abnormal")
                  << "\n";
    }

    return 0; // success
}
