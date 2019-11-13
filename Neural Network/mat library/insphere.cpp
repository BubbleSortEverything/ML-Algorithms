#include <stdio.h>
#include <stdlib.h>
#include "rand.h"
#include "mat.h"

// INPUT: <number dimensions> <number trials>
// OUTPUT: Y if in the unit sphere "." if not
// random points in d space selected and ask if point is in the sphere
// the Y and . give a viseral feel for frequency of inside sphere.
// OBSERVATION: as dimension goes up freq of inside sphere goes way down
int main(int argc, char *argv[])
{
    int dmax=atoi(argv[1]);
    int max=atoi(argv[2]);

    initRand();
    for (int i=0; i<max; i++) {
        double sum;

        sum = 0.0;
        for (int d=0; d<dmax; d++) {
            double r;

            r = randPMUnit();
            sum += r*r;
        }
        if (sum<1.0) printf("Y");
        else printf(".");
    }
    
    return 0;
}
