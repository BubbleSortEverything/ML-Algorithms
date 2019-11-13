/************************************************
 *
 *	Chaeun Kim
 *	02/24/2019
 *	CS475 - Multylayer Neural Network
 *
 *
 *************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "mat.h"
#include "rand.h"

#define ETA 0.003
#define MOMENTUM 0.9

using namespace std;

double transFunc(double);
double step(double);

int main()
{
	int numStep, numStride, numHidden;

	scanf("%d", &numStep);
	scanf("%d", &numStride);
	scanf("%d", &numHidden);

	Matrix dataSet, input;
	dataSet.read();
	dataSet.normalizeCols();
	Matrix target = dataSet.extract(numStep,0,dataSet.numRows()-numStep,dataSet.numCols());

	Matrix seriesInput = dataSet.seriesSampleCol(0, numStep, numStride);
	seriesInput.printfmt("Sampled Normalized Input:","",false);

	int sCol = seriesInput.numCols();
	int sRow = seriesInput.numRows();

	Matrix inputWithBias(sRow,sCol+1);
	inputWithBias.constantCol(sCol,1.0);
	seriesInput.extract(0,0,sRow,sCol,inputWithBias);

	Matrix weight1(inputWithBias.numCols(), numHidden+1), weight2(numHidden+1,1);

	initRand();
	weight1.rand(-0.25, 0.25);
	weight2.rand(-0.25, 0.25);
	Matrix updatew1(weight1), updatew2(weight2);
	updatew1.scalarMul(0.0); updatew2.scalarMul(0.0); // zero out these two vectors

	for (int i = 0; i < 3000; ++i)
	{
		Matrix hiddenLayer = inputWithBias.dot(weight1), output;
		hiddenLayer.map(&transFunc);
		output = hiddenLayer.dot(weight2);

		Matrix diff(output);
		diff.sub(target);
		diff.scalarMul(1/double(inputWithBias.numRows())); // deltao
		Matrix diffDotTw2 = diff.dotT(weight2);

		Matrix decrHidden(hiddenLayer);
		decrHidden.scalarPreSub(1.0);
		diffDotTw2.mul(hiddenLayer);
		diffDotTw2.mul(decrHidden); // deltah

		Matrix tempUpdateW1 = inputWithBias.Tdot(diffDotTw2);
		tempUpdateW1.scalarMul(ETA);
		updatew1.scalarMul(MOMENTUM);
		updatew1.add(tempUpdateW1);

		Matrix tempUpdateW2 = hiddenLayer.Tdot(diff);
		tempUpdateW2.scalarMul(ETA);
		updatew2.scalarMul(MOMENTUM);
		updatew2.add(tempUpdateW2);

		/*****************
			update weights
			******************/
		weight1.sub(updatew1);
		weight2.sub(updatew2);

	}
	Matrix hiddenLayer = inputWithBias.dot(weight1), output;
	hiddenLayer.map(&transFunc);
	output = hiddenLayer.dot(weight2);
	Matrix joinedOutput = output.joinRight(target);
	joinedOutput.printfmt("Est. and Target","",false);
	double dis = output.dist2(target);
	printf("Dist: %f\n",dis );

	return 0;

}

double transFunc(double x){
	return (1.0/(1.0 + exp(-1 * x)));
}

double step(double x){
	if(x > 0.5){
		return 1.0;
	}else{
		return 0.0;
	}
}