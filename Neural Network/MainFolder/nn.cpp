/*******************
 *  Name: Oshan Karki  
 *  Course: CS475(Machine Learning)
 *  Assignment: 1
 *  Description: A single layer perceptron
 ********************/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "rand.h"
#include "mat.h"

using namespace std;

/*  Prototypes
 */
double transferFunction(double x);

int main(int argc, char *argv[])
{
    initRand();     // initialize random function before using it.
    
    /*  Extracting number of input columns
     */
    int numOfInputs;
    scanf("%d", &numOfInputs);
    
    Matrix trainingMatrix;      // Training Matrix

    /* reading from the standard input
     */
    trainingMatrix.read();

    /* 
     *  extracting size of the training matrix
     *  and sub_matrix of actual output "y".
     *  input = number of inputs
     */
    unsigned int nCols, nRows;
    nRows = trainingMatrix.numRows();
    nCols = trainingMatrix.numCols();

    /* targetData row is equal to trainingMatrix row
     * targetData col is equal to trainingMatrix col - number of input col
     */
    unsigned int yRow = nRows; 
    unsigned int yCol = nCols - numOfInputs;

    /* Seprating training data into inputData and targetData.
     */
    Matrix inputData = trainingMatrix.extract(0, 0, nRows, numOfInputs);    //  Extracting input data
    //  Add bias to the input data
    //  Adding extra column for the bias column
    Matrix inputDataWithBias = new Matrix(inputData.numRows(), numOfInputs + 1, "inputDataWithBias");
    inputDataWithBias.constantCol(0, 1.0);  // adding bias
    for (int r = 0; r < inputDataWithBias.numRows(); r++) 
    {
        for (int c = 0; c < numOfInputs; c ++) 
        {
            inputDataWithBias.set(r,c+1, inputData.get(r, c));
        }
    }
    inputDataWithBias.normalizeCols();      // Normalizing the inputData


    Matrix targetData = trainingMatrix.extract(0, numOfInputs, yRow, yCol); //  Extracting training data

    /*  Reading the test Data
     */
    Matrix testData;
    testData.read();
    Matrix testDataWithBias = new Matrix(testData.numRows(), numOfInputs + 1, "testDataWithBias");
    testDataWithBias.constantCol(0, 1.0);   // adding bias
    for (int r = 0; r < testDataWithBias.numRows(); r++) 
    {
        for (int c = 0; c < numOfInputs; c ++) 
        {
            testDataWithBias.set(r,c+1, testData.get(r, c));
        }
    }

    /*  Makign a weighted matrix for weights
     */
    Matrix weight = new Matrix(inputDataWithBias.numCols(), targetData.numCols());
    weight.rand(0.0, 1.0);

    /*  Iterative Learning for-Loop
     */
    double eta = 0.1;    // learning rate
    for (int i = 0; i < 20000; ++i)
    {
        Matrix tempTargetData = new Matrix(targetData);     //  Declaring temprory matrix to store targetData
        Matrix weightedOutput = inputDataWithBias.dot(weight);      //  storing dot product of input data and weight in a weighted output 
        weightedOutput.map(transferFunction);               //  Mapping transfer function to each element of weightedOutput
        Matrix errorMatrix = tempTargetData.sub(weightedOutput);    //  error = target data - weighted output
        Matrix delta = inputDataWithBias.Tdot(errorMatrix);
        delta.scalarMul(eta);
        weight.add(delta);
    }

    /*  Testing the test data
     */
    printf("BEGIN TESTING\n");
    Matrix test;
    test = testDataWithBias.dot(weight);
    test.map(transferFunction);
    //test.print("Done Testing");
    for (int r = 0; r < testDataWithBias.numRows(); r++) {
        for (int c = 1; c < testDataWithBias.numCols(); c++) {
            printf("%.2f ", testDataWithBias.get(r, c));
        }
        for (int c = 0; c < test.numCols(); c++) {
            printf("%.2f ", test.get(r, c));
        }
        printf("\n");
    }
    return 0;
}

// Transfer function for the perceptron
double transferFunction(double x)
{
    double slope = 1.2;     //tan(45)
    double val = 1.0/(1.0 + exp(- slope * x));
    return val;
}