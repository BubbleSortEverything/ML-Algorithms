/******************************
 *	Name: Oshan Karki
 ******************************/

#include "rand.h"
#include "mat.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv){
	
	int kValue = stoi(argv[1]);		// Converting string to integer
	bool coloured, negative;

	/*	Check kValue
	 */
	if(kValue < 0){
		negative = true;
		kValue *= -1;
	}else{
		negative = false;
	}

	/*	Reading Orginal image file
	 */
	Matrix orginalPicture;
	cin >> coloured;
	orginalPicture.readImagePixmap("", "Image", coloured);
	//orginalPicture.printSize();

	/*	Check if picture needs to be transposed
	 */
	Matrix testPicture("Image");
	if(negative){
		testPicture = orginalPicture.transpose();
	}else{
		testPicture = orginalPicture;
	}

	/*	Center the data
	 */
	testPicture.printSize();
	Matrix meanCol("Mean");
	meanCol = testPicture.meanVec();
	meanCol.printSize();
	testPicture.subRowVector(meanCol);

	/*	Compute Covariance Matrix
	 */
	int R = testPicture.numRows();
	Matrix transposedTestPicture = testPicture.transpose();
	transposedTestPicture.scalarMul(1.0 / R);
	Matrix covariance = transposedTestPicture.dot(testPicture);

	/*	Compute Eigen System
	 */
	Matrix eigenVectors(covariance, "EigenVectors");
	Matrix eigenValues("EigenValues");
	eigenValues = eigenVectors.eigenSystem();
	eigenVectors.printSize();
	eigenValues.printSize();

	Matrix kEigenVectors(kValue, eigenVectors.numCols(), "K Eigen Vectors");
	kEigenVectors = eigenVectors.extract(0, 0, kValue, eigenVectors.numCols());
	Matrix kEigenValues(1, kValue, "K Eigen Values");
	kEigenValues = eigenValues.extract(0, 0, 1, kValue);

	/*	Translating Normalized data
	 */
	Matrix transposedKEigenVectors = kEigenVectors.transpose();
	Matrix compressedPicture("Encoded");
	compressedPicture = testPicture.dot(transposedKEigenVectors);
	compressedPicture.printSize();

	/*	Decompressing data
	 */
	Matrix decodedPicture("Decoded");
	decodedPicture = compressedPicture.dot(kEigenVectors);
	decodedPicture.addRowVector(meanCol);
	decodedPicture.printSize();

	if(negative == true){
		decodedPicture.transposeSelf();
	}

	if(coloured == false){
		decodedPicture.writeImagePgm("z.pgm", "Outputed pgm picture");
	}else{
		decodedPicture.writeImagePpm("z.ppm", "Outputed ppm picture");
	}

	double distance = decodedPicture.dist2(orginalPicture);
	double elements = (decodedPicture.numRows()) * (decodedPicture.numCols());
	distance = distance / elements;

	cout << "Per Pixel Dist^2: " << distance << endl;

	return 0;
}