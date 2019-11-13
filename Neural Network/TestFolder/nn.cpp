#include<cstdio>
#include<cmath>
#include "mat.h"

double transferFunction(double x);

int main() {
    initRand();

    // Read in inputs
    int numInputs;
    scanf("%d", &numInputs);
    Matrix raw = new Matrix();
    raw.read();

    // Setup input matrix
    Matrix inputs = new Matrix(raw.numRows(), numInputs + 1, "inputs");
    inputs.mapCol(0, [](double d) -> double {return 1.0;}); // Add bias

    //printing the input with bias
    //inputs.print("Input with bias");

    // Copy raw read over to new input matrix
    for (int r = 0; r < inputs.numRows(); r++) {
        for (int c = 0; c < numInputs; c ++) {
            inputs.set(r,c+1, raw.get(r, c));
        }
    }
    inputs.normalizeCols();

    Matrix targets = raw.extract(0,numInputs,0,0);
    targets.setName("targets");

    // Read in test data
    raw = new Matrix();
    raw.read();

    printf("BEGIN TESTING\n");

    // Setup test matrix
    Matrix tests = new Matrix(raw.numRows(), raw.numCols() + 1, "tests");
    tests.mapCol(0, [](double d) -> double {return 1.0;}); // Add bias

    // Copy raw read over to tests
    for (int r = 0; r < raw.numRows(); r++) {
        for (int c = 0; c < tests.numCols() - 1; c++) {
            tests.set(r,c+1, raw.get(r, c));
        }
    }



    // Generate weights
    Matrix weights = new Matrix(inputs.numCols(), targets.numCols());
    weights.setName("weights");
    weights.rand(0.0, 1.0);

    // Learning loop

    double eta = 0.1;
    for (int i = 0; i < 10000; i++) {
        Matrix targetTemp = new Matrix(targets);
        Matrix guess = inputs.dot(weights);
        guess.map(transferFunction);
        Matrix errors = targetTemp.sub(guess);
        Matrix deltas = inputs.Tdot(errors);
        deltas.scalarMul(eta);
        weights.add(deltas);
    }

    // Done learning, show tests
    Matrix testOutput = tests.dot(weights);
    testOutput.map(transferFunction);
    testOutput.print("ok");
    for (int r = 0; r < tests.numRows(); r++) {
        for (int c = 1; c < tests.numCols(); c++) {
            printf("%.2f ", tests.get(r, c));
        }
        for (int c = 0; c < testOutput.numCols(); c++) {
            printf("%.2f ", testOutput.get(r, c));
        }
        printf("\n");
    }
    return 0;
}

double transferFunction(double x) {
    double slope = 5.0;
    return 1.0/(1.0 + exp(-slope*x));
}