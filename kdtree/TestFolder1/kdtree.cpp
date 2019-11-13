/*  Name: Oshan Karki
 *  Course: CS475 (Machine Learning)
 *  Professor: Robert Baker Heckendorn
 *  Assignment: 6
 *  Description: Kdtree Algorithm
 */

#include "mat.h"
#include <float.h>
#include <cmath>

void build(Matrix &,int,int,int);
double myDist(Matrix &,int,Matrix &);
void nearestAux(Matrix &,Matrix &,int,int,int,double &,int &);
int nearest(Matrix &,Matrix &);

int main()
{
    Matrix items("item");      // the data item to be searched for
    Matrix tree("kdtree");     // a kdtree as a matrix
    SymbolNumMap labels;       // labels for a row labeled matrix (see the matrix library)
    int closestRow;               // the row that is closestRow to item using the measure myDist

    // read in labeled training data
    labels = tree.readLabeledRow();

    // build tree
    build(tree, 0, tree.numRows()-1, 1);

    // display tree for comparison in tests
    printf("KDTree version of matrix\n");
    tree.printLabeledRow(labels);
    printf("\n");

    // read in the items we wish to look up
    items.read();

    MatrixRowIter a(&items);
    for (Matrix *item = a.rowBegin(); a.rowNotEnd(); a.rowNext()) {

        // display the item
        printf("\nSOLVE: ");
        item->writeLine(0);
        printf("\n");

        // use kdtree to find row number of closestRow in data
        closestRow = nearest(tree, *item);

        printf("ANS: ");
        printf("%d %s ", closestRow, labels.getStr(tree.get(closestRow, 0)).c_str());
        tree.subMatrix(closestRow, 1, 1, 0).writeLine(0);
        printf("\n");
    }

    return 0;
}
// Uses a Matrix to create a balanced binary tree from rows of data
// by repeatedly sorting on different columns.   The first column
// is the label of the row which is a known classification or property of the
// data in the rest of the row.   This fits well with the labeled row version
// supported by the matrix library.
//
// tree: tree to be built in Matrix form
// rowstart, rowend: the range of rows to sort
// c: the column to sort on
void build(Matrix &tree, int rowstart, int rowend, int c)
{
    int split;

    if (c>=tree.numCols()) c=1;

    tree.sortRowsByCol(c, rowstart, rowend);

    split = (rowstart + rowend)/2;
    if ((split-1)-rowstart>0) build(tree, rowstart, split-1, c+1);
    if (rowend-(split+1)>0) build(tree, split+1, rowend, c+1);
}
// IMPORTANT: must be full real distance measure (not the square of
// the distance) since short cut relies on max abs(difference) as a
// distance in one dimension. ASSUMES matrix is a labeled matrix.
double myDist(Matrix &x, int r, Matrix &item) {
    return sqrt(x.subMatrix(r, 1, 1, 0).dist2(item));
}
// tree is a kd-tree and item is a row matrix to look up.
void nearestAux(Matrix &tree,   // the kdtree matrix
                Matrix &item,   // the row
                int rowstart,   // beginning row of region to search (subtree)
                int rowend,     // ending row of region to search
                int c,          // column or feature to compare
                double &best,   // the distance to nearest point found so far
                int &bestrow) {  // the row of the nearest point found so far
    c = (c >= tree.numCols()) ? 1 : c;
    if(rowstart > rowend)
        return;
    else if((rowend - rowstart) >= 3){
        double dis;
        for(int i = rowstart; i <= rowend; i++){
            dis = myDist(tree,i,item);
            if(dis < best){
                best = dis;
                bestrow = i;
            }
        }
    }
    else{
        int mid = (rowstart + rowend) / 2;
        if(item.get(rowstart,c-1) < tree.get(mid,c))
            nearestAux(tree,item,rowstart,mid,c,best,bestrow);
        else if(item.get(rowstart,c-1) > tree.get(mid,c))
            nearestAux(tree,item,mid+1,rowend,c,best,bestrow);
        else
            nearestAux(tree,item,rowstart,rowend,++c,best,bestrow);
    }

    return;
}
int nearest(Matrix &tree, Matrix &item) {
    double best;
    int bestRow;

    best = DBL_MAX;  // minimum value found so far (maximum double value from float.h)
    bestRow = -1;    // row of best

    nearestAux(tree, item, 0, tree.numRows()-1, 1, best, bestRow);

    return bestRow;
}