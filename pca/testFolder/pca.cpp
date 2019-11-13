#include "rand.h"
#include "mat.h"
#include <iostream>

using namespace std;

int main(int argc, char ** argv){

    int k_value = stoi(argv[1]);
    bool color, negative;

    if(k_value < 0){
        negative = true;
        k_value = k_value * -1;
    }
    else
    {
        negative = false;
    }
    
    // Get original Pic from stdi
    Matrix Originial_Pic;
    cin >> color;
    Originial_Pic.readImagePixmap("", "Pic", color);
    Originial_Pic.printSize();

    // Check to see if pic needs to be transposed
    Matrix Working_Pic;

    if (negative) {
        Working_Pic = Originial_Pic.transpose();
    }
    else
    {
       Working_Pic = Originial_Pic;
    }
    

    // Center the data
    Matrix ColMean("Mean");
    ColMean = Working_Pic.meanVec();
    ColMean.printSize();
    Working_Pic.subRowVector(ColMean);

    // Compute Covariance Matrix
    int R = Working_Pic.numRows();
    Matrix Working_Pic_Trans = Working_Pic.transpose();
    Working_Pic_Trans.scalarMul(1.0 / R);
    Matrix Covariance = Working_Pic_Trans.dot(Working_Pic);
    
    // Compute Eigen System
    Matrix Eigen_Vectors(Covariance, "EiganVectors");
    Matrix Eigen_Values("EigenValues");
    Eigen_Values = Eigen_Vectors.eigenSystem();
    Eigen_Vectors.printSize();
    Eigen_Values.printSize();

    Matrix K_Eigen_Vectors(k_value, Eigen_Vectors.numCols(), "K Eigen Vectors");
    K_Eigen_Vectors = Eigen_Vectors.extract(0, 0, k_value, Eigen_Vectors.numCols());
    Matrix K_Eigen_Values(1, k_value, "K Eigen Values");
    K_Eigen_Values = Eigen_Values.extract(0, 0, 1, k_value);

    // Translate Normalized Data
    Matrix K_Eigen_Vectors_Trans = K_Eigen_Vectors.transpose();
    Matrix Compressed_Pic("Encoded");
    Compressed_Pic = Working_Pic.dot(K_Eigen_Vectors_Trans);
    Compressed_Pic.printSize();

    // Recovering Data from compressed data
    Matrix Rotated_Pic("Decoded");
    Rotated_Pic = Compressed_Pic.dot(K_Eigen_Vectors);
    Rotated_Pic.addRowVector(ColMean);
    Rotated_Pic.printSize();

    // Transpose if needed
    if (negative == true) {
        Rotated_Pic.transposeSelf();
    }
    

    // Write Out recovered pic
    if (color == false) {
        Rotated_Pic.writeImagePgm("z.pgm", "Outputed pgm picture");
    }
    else
    {
        Rotated_Pic.writeImagePpm("z.ppm", "Outputed ppm picture");
    }
    
    // Compute distance of picture 
    double distance = Rotated_Pic.dist2(Originial_Pic);
    double rows = Rotated_Pic.numRows();
    double cols = Rotated_Pic.numCols();
    double elements = rows * cols;
    distance = distance / elements;

    cout << "Per Pixel Dist^2: " << distance << endl;

    return 0;
}