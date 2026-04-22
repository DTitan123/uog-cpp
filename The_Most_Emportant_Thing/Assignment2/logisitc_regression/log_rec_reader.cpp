// log_rec_reader.cpp

// ECG CSV reader.
//
// Opens ../data/ecg.csv, skips the header row, then parses
// each remaining line into a feature vector (first 140 cols)
// and a binary label (last col: 0 = abnormal, 1 = normal).
// Prints a summary so we know the file was read correctly.


#include <fstream>    // std::ifstream
#include <iostream>   // std::cout, std::cerr
#include <sstream>    // std::istringstream
#include <string>     // std::string
#include <vector>     // std::vector


int main() {

    // Open the CSV file. Path is relative: run from the src/ directory
    // so that ../data/ resolves to the data folder next to src/.
    std::ifstream file("../data/ecg.csv");
    if (!file) {
        std::cerr << "Error: could not open ../data/ecg.csv\n";
        return 1;
    }

    // `inputs` holds feature vectors (one per sample, 140 features each).
    // `outputs` holds the binary label for each sample.
    std::vector<std::vector<double>> inputs;
    std::vector<double> outputs;

    std::string line;

    // Skip the header row (column names).
    if (!std::getline(file, line)) {
        std::cerr << "Error: file appears to be empty\n";
        return 1;
    }

    // Read every data row.
    while (std::getline(file, line)) {
        if (line.empty()) continue; // skip blank lines

        std::istringstream linestream(line);
        std::string cell;
        std::vector<double> row;

        // Split on commas; convert each token to double.
        // Empty cells and non-numeric tokens become 0.0.
        while (std::getline(linestream, cell, ',')) {
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

        // Convention: last column = label y (0 or 1).
        outputs.push_back(row.back());
        row.pop_back();
        inputs.push_back(row);
    }

    if (inputs.empty()) {
        std::cerr << "Error: no data read from file\n";
        return 1;
    }

    // Summarise the loaded dataset.
    std::cout << "Samples loaded  : " << inputs.size()    << "\n";
    std::cout << "Features/sample : " << inputs[0].size() << "\n";

    // Count class distribution.
    int n_normal = 0, n_abnormal = 0;
    for (double label : outputs) {
        if (label == 1.0) ++n_normal;
        else              ++n_abnormal;
    }
    std::cout << "Labels = 0 (abnormal): " << n_abnormal << "\n";
    std::cout << "Labels = 1 (normal)  : " << n_normal   << "\n";

    return 0; // success
}
