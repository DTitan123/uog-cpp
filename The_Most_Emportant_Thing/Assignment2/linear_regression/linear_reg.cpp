#include <fstream>
// Simple, beginner-friendly CSV reader.
// Reads ../data/concrete.csv, treats the last column as the target,
// and prints how many samples and features were loaded.

#include <fstream>   // for std::ifstream
#include <iostream>  // for std::cout, std::cerr
#include <sstream>   // for std::istringstream
#include <string>    // for std::string
#include <vector>    // for std::vector

int main() {
    // Try to open the CSV file (relative path).
    std::ifstream file("../data/concrete.csv");
    if (!file) {
        std::cerr << "Error: could not open ../data/concrete.csv\n";
        return 1; // non-zero = error
    }

    // Containers for features (inputs) and the target (outputs).
    std::vector<std::vector<double>> inputs;
    std::vector<double> outputs;

    std::string line;

    // Read and discard the first line if it is a header (column names).
    // If your CSV has no header, remove the next line so you don't skip data.
    if (!std::getline(file, line)) {
        std::cerr << "Error: file appears to be empty\n";
        return 1;
    }

    // Read each remaining line and parse numbers separated by commas.
    while (std::getline(file, line)) {
        if (line.empty()) continue; // skip blank lines
        std::istringstream linestream(line);
        std::string cell;
        std::vector<double> row;

        // Split the line on commas and convert tokens to double.
        while (std::getline(linestream, cell, ',')) {
            if (cell.empty()) {
                // If a cell is empty, treat it as 0.0 for simplicity.
                row.push_back(0.0);
            } else {
                try {
                    row.push_back(std::stod(cell));
                } catch (...) {
                    // If conversion fails, push 0.0 and continue (simple fallback).
                    row.push_back(0.0);
                }
            }
        }

        if (row.empty()) continue; // nothing parsed on this line

        // Last value is the target (output); remove it from features.
        outputs.push_back(row.back());
        row.pop_back();
        inputs.push_back(row);
    }

    if (inputs.empty()) {
        std::cout << "No samples loaded\n";
        return 1;
    }

    // Print dataset summary.
    std::cout << "Samples loaded: " << inputs.size() << "\n";
    std::cout << "Features per sample: " << inputs[0].size() << "\n";

    return 0; // success
}
