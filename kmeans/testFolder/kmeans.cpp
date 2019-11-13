/*******************************************
 *	
 *	Oshan Karki
 *	CS475 - Assignment 5
 *	04/05/2019
 *
 *******************************************/

#include "rand.h"
#include "mat.h"
#include <math.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv){

	if(argc < 3){
		cout << "too few arguments!\n";
		exit(-1);
	}
	else if(argc > 3){
		cout << "too many arguments!\n";
		exit(-1);
	}

	int k = stoi(argv[1]);
	int t = stoi(argv[2]);

	Matrix data;
	data.read();
	int dCol = data.numCols();
	int dRow = data.numRows();

	initRand();
	int it = 0;
	while(it < t){
		Matrix centroids(k,dCol,0.0,"");
		double min,max;
		for(int j = 0; j < dCol; j++){
			min = data.minCol(j);
			max = data.maxCol(j);

			centroids.randCol(j,min,max);
		}

		double minD = 10000.0;
		double distance = -1.0;
		while( distance != 0.0 ){
			Matrix classification(dRow,1,0.0,"");
			double dis = 0.0, minDis = 1000.0, clusterIndex = 0;
			for(int i = 0; i < dRow; i++){
				for(int j = 0; j < k; j++){
					for(int z = 0; z < dCol; z++){
						dis += pow(centroids.get(j,z)-data.get(i,z), 2);
					}
					dis = sqrt(dis);
					if(dis < minDis){
						minDis = dis;
						clusterIndex = j;
					}
				}
				classification.set(i,0,clusterIndex);
				minDis = 1000.0;
			}

			Matrix newCentroids(k,dCol,0.0,"");
			int nClusteredData = 0;
			for(int i = 0; i < k; i++){
				Matrix tmpCluster(1,dCol,0.0,"");
				for(int j = 0; j < dRow; j++){
					if(classification.get(j,0) == i){
						nClusteredData++;

						for(int z = 0; z < dCol; z++){
							tmpCluster.set(0,z,tmpCluster.get(0,z)+data.get(j,z));
						}
					}
				}
				if(nClusteredData == 0){
					for(int c = 0; c < dCol; c++){
						min = data.minCol(c);
						max = data.maxCol(c);
						newCentroids.set(i,c, randUnit()*(max-min) + min);
					}
				}else{
					for(int c = 0; c < dCol; c++){
						newCentroids.set(i,c,tmpCluster.get(0,c)/nClusteredData);
					}
				}
				nClusteredData = 0;
			}
			//newCentroids.print("NEW");
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



