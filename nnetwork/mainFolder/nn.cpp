/*	Name: Oshan Karki		Date: March 19, 2019
 */

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "mat.h"
#include "rand.h"

using namespace std;

/*	Function Prototypes
 */
double transferFunction(double);
double step(double);

int main()
{
	int numOfSteps, numOfStrides, numOfHidden;

	/*	Scannig the number of steps, strides and hidden layer
	 */
	scanf("%d", &numOfSteps);
	scanf("%d", &numOfStrides);
	scanf("%d", &numOfHidden);

	/*	Reading input data
	 */
	Matrix dataSet, input;
	dataSet.read();
	Matrix target = dataSet.extract(numOfSteps,0,dataSet.numRows()-numOfSteps,dataSet.numCols());
	Matrix targetMinMax = target.normalizeCols();

	dataSet.normalizeCols();
	Matrix seriesInput = dataSet.seriesSampleCol(0, numOfSteps, numOfStrides);	//	extracting series input
	seriesInput.printfmt("Sampled Normalized Input:","",false);

	/*
	Matrix input_w_bias = Matrix(input_matrix.numRows(), input_matrix.numCols(), "input with bias");
    input_w_bias.insert(input_matrix, 0, 0);
    input_w_bias.constantCol(steps, -1.0);
    */

	input = seriesInput.extract(0,0,seriesInput.numRows(),(seriesInput.numCols() - dataSet.numCols()));	//	extracting input data from series input
	Matrix weight_1(input.numCols(), numOfHidden + 1);
	Matrix weight_2(numOfHidden + 1, 1);

	initRand();	// initializing random functions
	weight_1.rand(-0.25, 0.25);
	weight_2.rand(-0.25, 0.25);
	Matrix updatedWeight_1(weight_1), updatedWeight_2(weight_2);
	updatedWeight_1.scalarMul(0.0); updatedWeight_2.scalarMul(0.0); // zero out these two vectors

	double eta = 0.004;	//	In a compressed data always use a small eta possible
	double momentum = 0.9;

	/*	Iterative learning loop
	 */
	for (int i = 0; i < 40000; ++i)
	{
		Matrix hiddenLayer = input.dot(weight_1), output;
		hiddenLayer.map(&transferFunction);
		output = hiddenLayer.dot(weight_2);

		Matrix diffMatrix(output);
		diffMatrix.sub(target);
		diffMatrix.scalarMul(1/double(input.numRows()));
		Matrix dotTransposeddiffMatrixWithWeight_2 = diffMatrix.dotT(weight_2);

		Matrix decrHidden(hiddenLayer);
		decrHidden.scalarPreSub(1.0);
		dotTransposeddiffMatrixWithWeight_2.mul(hiddenLayer);
		dotTransposeddiffMatrixWithWeight_2.mul(decrHidden); // deltah

		Matrix tempupdatedWeight_1 = input.Tdot(dotTransposeddiffMatrixWithWeight_2);
		tempupdatedWeight_1.scalarMul(eta);
		updatedWeight_1.scalarMul(momentum);
		updatedWeight_1.add(tempupdatedWeight_1);

		Matrix tempupdatedWeight_2 = hiddenLayer.Tdot(diffMatrix);
		tempupdatedWeight_2.scalarMul(eta);
		updatedWeight_2.scalarMul(momentum);
		updatedWeight_2.add(tempupdatedWeight_2);

		weight_1.sub(updatedWeight_1);
		weight_2.sub(updatedWeight_2);
	}

	Matrix hiddenLayer = input.dot(weight_1), output;
	hiddenLayer.map(&transferFunction);
	output = hiddenLayer.dot(weight_2);
	output.unnormalizeCols(targetMinMax);
	target.unnormalizeCols(targetMinMax);

	Matrix mergedOutput = output.joinRight(target);
	mergedOutput.printfmt("Est. and Target","",false);
	double dis = output.dist2(target);
	printf("Dist: %f\n",dis );
	
	return 0;
}

double transferFunction(double x){
	return (1.0/(1.0 + exp(-1 * x)));
}

double step(double x){
	if(x > 0.5){
		return 1.0;
	}else{
		return 0.0;
	}
}