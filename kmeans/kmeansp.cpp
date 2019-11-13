/*	Name: Oshan Karki
 *	Course: CS475 (Machine Learning)
 *	Professor: Robert Baker Heckendorn
 *	Assignment: 5
 *	Description: kmeansp.cpp
 */

#include "rand.h"
#include "mat.h"
#include <math.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv){


	/*	Check for valid arguments
	 */
	if(argc < 3){
		cout << "Invalid number of Arguments!\n";
		exit(-1);
	}
	else if(argc > 3){
		cout << "Invalid number of Arguments!\n";
		exit(-1);
	}

	int k = stoi(argv[1]);
	int t = stoi(argv[2]);

	Matrix inputPoints;
	inputPoints.read();
	int inputCol = inputPoints.numCols();
	int inputRow = inputPoints.numRows();
	
	Matrix centroids(k,inputCol,0.0,"");

	initRand();
	int it = 0;

	while(it < t){

		centroids.sampleWithout(inputPoints);
		double min,max;
		double minD = 10000.0;
		double distance = -1.0;

		while( distance != 0.0 ){

			Matrix classification(inputRow,1,0.0,"");
			double dis = 0.0, minDist = 1000.0, clusterIndex = 0;

			for(int i = 0; i < inputRow; i++){
				for(int j = 0; j < k; j++){
					for(int z = 0; z < inputCol; z++){
						dis += pow(centroids.get(j,z)-inputPoints.get(i,z), 2);
					}
					dis = sqrt(dis);
					if(dis < minDist){
						minDist = dis;
						clusterIndex = j;
					}
				}
				classification.set(i,0,clusterIndex);
				minDist = 1000.0;
			}

			Matrix newCentroids(k,inputCol,0.0,"");
			int numOfClusteredInputPoints = 0;
			for(int i = 0; i < k; i++){
				Matrix tmpCluster(1,inputCol,0.0,"");
				for(int j = 0; j < inputRow; j++){
					if(classification.get(j,0) == i){
						numOfClusteredInputPoints++;

						for(int z = 0; z < inputCol; z++){
							tmpCluster.set(0,z,tmpCluster.get(0,z)+inputPoints.get(j,z));
						}
					}
				}
				if(numOfClusteredInputPoints == 0){
					for(int c = 0; c < inputCol; c++){
						min = inputPoints.minCol(c);
						max = inputPoints.maxCol(c);
						newCentroids.set(i,c, randUnit()*(max-min) + min);
					}
				}else{
					for(int c = 0; c < inputCol; c++){
						newCentroids.set(i,c,tmpCluster.get(0,c)/numOfClusteredInputPoints);
					}
				}
				numOfClusteredInputPoints = 0;
			}
			distance = newCentroids.dist2(centroids);
			minD = (distance < minD && distance != 0.0) ? distance : minD;

			centroids = newCentroids;

		}
		centroids.sortRows();
		Matrix tmp = centroids.transpose();
		distance = 0.0;
		minD = 10000.0;
		for(int i = 0; i < k; i++){
			for(int j = 0; j < k; j++){
				if(i!=j){
					distance = centroids.dist2(j,i,tmp);
					minD = (distance < minD) ? distance : minD;
				}
			}
		}

		centroids.printfmt("Points:","",false);
		printf("K: %d MinD: %.4f\n", k,minD);

		it++;
	}

	return 0;

}