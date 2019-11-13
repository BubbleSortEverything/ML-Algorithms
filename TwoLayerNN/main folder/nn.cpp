/*	Program: nn.cpp
 *	Name: Oshan Karki
 *	Course: CS475 (Machine Learning)
 *	Professor: Dr. Heckendorn
 *	Description: Two layer Neural Network
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include "mat.h"
#include "rand.h"

using namespace std;

/*	Auxilary Functions
 */
double transferFunction(double x){
	double slope = 1.0;
	return (1.0/(1.0 + exp(-4 * slope * x)));
}

double oneMinus(double x){
	return 1 - x;
}

double step(double x){
	if(x > 0.5){
		return 1.0;
	}else{
		return 0.0;
	}
}

double addBias(double x){
	return 1.0;
}


/*	Main Function
 */
int main(int argc, char const *argv[])
{
	int numOfInputs, numOfHiddenNodes, numOfClasses;

	scanf("%d", &numOfInputs);
	scanf("%d", &numOfHiddenNodes);
	scanf("%d", &numOfClasses);

	/*	reading the data file
	 */
	Matrix unrefined;
	unrefined.read();
	//unrefined.print();

	/*	Setting up target matrix
	 */
	Matrix targetMatrix;
	targetMatrix = unrefined.extract(0, numOfInputs, unrefined.numRows(), unrefined.numCols() - numOfInputs);
	//targetMatrix.print("targetMatrix");

	/*	setting up training matrix;
	 */
	Matrix trainingMatrix = unrefined.extract(0, 0, unrefined.numRows(), numOfInputs);
	//trainingMatrix.print("trainingMatrix");	

	/*	adding bias to training matrix
	 *	normalizing columns and shuffle the data
	 */
	Matrix trainingMatrixWithBias = new Matrix(trainingMatrix.numRows(), numOfInputs + 1, "trainingMatrixWithBias");
	trainingMatrixWithBias.insert(trainingMatrix, 0, 0);
	trainingMatrixWithBias.mapCol(trainingMatrixWithBias.numCols() - 1, &addBias); // initializing bias as -1

	trainingMatrixWithBias.normalizeCols();
	trainingMatrixWithBias.shuffle();

	/*	Setting up weight matrix
	 */
	Matrix weightMatrix = new Matrix(numOfHiddenNodes + 1, targetMatrix.numCols());
	initRand();
	weightMatrix.rand(-1.0, 1.0);
	//weightMatrix.print("weight matrix");

	/*	Settting up vector matrix
	 */
	Matrix vectorMatrix = new Matrix(trainingMatrixWithBias.numCols(), numOfHiddenNodes);
	vectorMatrix.rand(0.0, 1.0);

	/*	Iterative Learning Loop
	 */
	double eta = 0.1;
	for(int i = 0; i < 30000; i++){

		/* Forward Propagtion
		 */

		Matrix tempTarget = new Matrix(targetMatrix);
		Matrix hiddenLayer = trainingMatrixWithBias.dot(vectorMatrix);
		hiddenLayer.map(&transferFunction);
		Matrix hiddenLayerWithBias = new Matrix(hiddenLayer.numRows(), hiddenLayer.numCols() + 1);
		hiddenLayerWithBias.insert(hiddenLayer, 0, 0);
		hiddenLayerWithBias.mapCol(hiddenLayerWithBias.numCols() - 1, &addBias);
		Matrix output = hiddenLayerWithBias.dot(weightMatrix);
		output.map(&transferFunction);

		/*	Backward Propagation
		 */

		/*	Creating a dy matrix with output matrix and target matrix */
		Matrix dy = new Matrix(output);
		dy.sub(tempTarget);
		Matrix oneMinusOutput = new Matrix(output);
		oneMinusOutput.map(&oneMinus);
		dy.mul(output);
		dy.mul(oneMinusOutput);

		/*	creating a dHiddenBias matrix with dy, hiddenLayerWithBias and weight matrix */
		Matrix dHiddenBias = Matrix(hiddenLayerWithBias);
		Matrix oneMinusHiddenBias = new Matrix(hiddenLayerWithBias);
		oneMinusHiddenBias.map(&oneMinus);
		Matrix transposedWeight = weightMatrix.transpose();
		Matrix dyDotWeightMatrix = dy.dot(transposedWeight);
		dHiddenBias.mul(oneMinusHiddenBias);
		dHiddenBias.mul(dyDotWeightMatrix);

		/*	Update Nodes
		 */
		Matrix hiddenLayerDot_dy = hiddenLayerWithBias.Tdot(dy);
		hiddenLayerDot_dy.scalarMul(eta);
		weightMatrix.sub(hiddenLayerDot_dy);

		Matrix dHidden = dHiddenBias.extract(0, 0, dHiddenBias.numRows(), dHiddenBias.numCols() - 1);

		Matrix transposedTrainingDataWithBiasWith_dHidden = trainingMatrixWithBias.Tdot(dHidden);
		transposedTrainingDataWithBiasWith_dHidden.scalarMul(eta);
		vectorMatrix.sub(transposedTrainingDataWithBiasWith_dHidden);		

	}


	/*	Setting up test matrix
	 */
	Matrix testMatrix;
	testMatrix.read();

	/*	setting up test target matrix
	 */
	Matrix testTarget = testMatrix.extract(0, numOfInputs, testMatrix.numRows(), testMatrix.numCols() - numOfInputs);
	//testTarget.print("test target");

	/*	setting up input matrix
	 */
	Matrix inputMatrix;
	inputMatrix = testMatrix.extract(0, 0, testMatrix.numRows(), testMatrix.numCols() - testTarget.numCols());

	/*	adding bias to input data
	 */
	Matrix inputWithBias = new Matrix(testMatrix.numRows(), numOfInputs + 1, "inputWithBias");
	inputWithBias.insert(inputMatrix, 0, 0);
	inputWithBias.mapCol(inputWithBias.numCols() - 1, &addBias);
	//inputWithBias.shuffle();
	inputWithBias.normalizeCols();

	/*	Testing 
	 */
	Matrix temp;
	temp = inputWithBias.dot(vectorMatrix);
	temp.map(&transferFunction);
	Matrix tempWithBias = new Matrix(temp.numRows(), temp.numCols() + 1);
	tempWithBias.insert(temp, 0, 0);
	tempWithBias.mapCol(tempWithBias.numCols() - 1, &addBias);
	Matrix result = tempWithBias.dot(weightMatrix);
	result.map(&transferFunction);

	testTarget.printfmt("Target", "", false);
	result.map(&step);
	result.printfmt("Predicted", "", false);

	/*	setting up confusion matrix
	 */
	Matrix tempResult = new Matrix(result);
	tempResult.map(&step);


	for(int i = 0; i < testTarget.numCols(); i++){
		Matrix confusionMatrix = new Matrix(numOfClasses, numOfClasses, "confusionMatrix");
		confusionMatrix.constant(0);
		for(int j = 0; j < testTarget.numRows(); j++){
			if(testTarget.get(j, i) == 1){
				if(tempResult.get(j, i) == 1){
					confusionMatrix.inc(1, 1);
				}else{
					confusionMatrix.inc(0, 1);
				}
			}else{
				if(tempResult.get(j, i) == 1){
					confusionMatrix.inc(1, 0);
				}else{
					confusionMatrix.inc(0, 0);
				}
			}
		}
	confusionMatrix.printfmt("Confusion Matrix", "", false);
	}


	return 0;
}