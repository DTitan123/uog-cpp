// logistic_regression.hpp
// Binary Logistic Regression for ENG3091 Assignment 02
//
// This file is used to implement a LogisticRegression class for binary classification.
// It is applied to the ECG dataset (ecg.csv) to predict whether a patient is normal (1) or abnormal (0).
//
//
// Class structure:
//   Private:
//     - weights, bias, lambda : model parameters
//     - sigmoid()             : maps z between 0 and 1 
//
//   Public:
//     - fit()           : trains the model using gradient descent
//     - predict_proba() : returns probability of belonging to class 1
//     - predict()       : returns predicted class label (0 or 1)
//     - score()         : returns accuracy on a given dataset
//  Class structure was first to be defined as a skeleton to then subsequently each function was then defined.

// Consistent with linear_reg.hpp and log_main.cpp in namespace and coding style.

#pragma once //reduces strain on compiler: if this file has been already considered it will just be skipped
 
#include <vector> 
#include <cmath> // for the sigmoid function
#include "linear_reg.hpp"  // access abstract class

// namespaces included as unique reference of the class.
// defined previously in linear_reg.hpp by mirrorring python code provided in assignment brief
namespace sklearn_cpp {
namespace linear_model {
class LogisticRegression : public sklearn_cpp::linear_model::BaseModel {
private:
    std::vector<double> weights;
    double bias;
    double lambda; // regularisation strength
    double sigmoid(double z) const //function to calculate sigmoid (1 / (1 + e^-z))
    {
        return 1.0 / (1.0 + std::exp(-z));
    }

public:
// constructor: lambda_reg controls L2 regularisation strength (default 0.01)
// explicit prevents implicit conversions from double to LogisticRegression
explicit LogisticRegression(double lambda_reg = 0.01)
    : bias { 0.0 }, lambda { lambda_reg } {}

// fit() receives the X matrix and the Y labels and sets the learning rate 
// and the number of iterations. this thus finds the best weight and bias
void fit(const std::vector<std::vector<double>>& X, const std::vector<double>& y, double learning_rate = 1e-4, int iterations = 1000) 
{
    // initialise variables
    const int m { static_cast<int>(X.size()) };      // number of samples
    const int n { static_cast<int>(X[0].size()) };   // number of features
    weights.assign(n, 0.0);
    bias = 0.0; 
    double error {};
    double weight_contribution {}; //refers to how much weight affects the error
    std::vector<double> weight_contribution_accumulated;

    for(int i {}; i < iterations; ++i){
        weight_contribution_accumulated.assign(n, 0.0);
        double bias_accumulated { 0.0 }; 
        
        for(int j {}; j < m; ++j){
            error = predict_proba(X[j]) - y[j];
            for (int k {}; k < n; ++k){
                weight_contribution = error * X[j][k];
                weight_contribution_accumulated[k] += weight_contribution;
            }
            bias_accumulated += error;
        }

        // update bias and weight
        for (int k {}; k < n; ++k){
            weights[k] -= learning_rate * ((1.0/m) * weight_contribution_accumulated[k] + 2*lambda*weights[k]);
        }
        bias = bias - learning_rate * ((1.0/m) * bias_accumulated + 2*lambda*bias);
    }
}

// returns probability of a given X being in class 1 
double predict_proba(const std::vector<double>& x) const
{   
    double z { bias };
    // using a for loop for ease to cycle through a known number of data points 
    // (weights.size=x.size so no need to have both conditions.)
    for (int i { 0 }; i < static_cast<int>(weights.size()); ++i){
        z += weights[i] * x[i];
    }
    return sigmoid(z);
}

// if the probability is >= 0.5, return 1 otherwise return 0
int predict(const std::vector<double>& x) const{
    int probability {};
    // simple if-else as only two options available could use boolean 
    // but if/else already within the other scripts so for consistency purposes it is used
    if(predict_proba(x) >= 0.5)
    {
        probability = 1;
    }
    else
    {
        probability = 0;
    }
    return probability;   
}

// takes x and y as inputs to then return the accuracy of the model as float
double score(const std::vector<std::vector<double>>& X, const std::vector<double>& y) const
{ 
    int correct_counter { 0 }; // keeps count of correct predictions
    double accuracy {};        // variable to store the final accuracy
    
    // loop to cycle through data, if prediction=label we increment
    // for used rather than while, as number of data points known  
    for(int i {}; i < static_cast<int>(X.size()); ++i)
    { 
        if (predict(X[i]) == y[i])
        { 
            correct_counter++;
        }
        else {
            continue;
        }
    }
    // static_cast needed as result needed in double and we are dividing integers
    accuracy = static_cast<double>(correct_counter) / static_cast<int>(X.size());
    return accuracy; 
}

};
} // namespace linear_model
} // namespace sklearn_cpp