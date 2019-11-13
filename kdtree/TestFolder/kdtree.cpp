#include "mat.h"
#include <float.h>
#include<iostream>

using namespace std;

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
//
//
//

    return;
}


int nearest(Matrix &tree, Matrix &item) {
    double best;
    int bestrow;

    best = DBL_MAX;  // minimum value found so far (maximum double value from float.h)
    // Initiating the bestrow from 0.
    bestrow = 0;    // row of best
    double x_tree, y_tree, z_tree, x_item, y_item, z_item, dist = 0;
    // Getting the x and y coordinates of the sample item.
    if(item.numCols() == 2)
    {
        x_item = item.get(0, 0);
        y_item = item.get(0, 1);
    }
    else
    {
        x_item = item.get(0, 0);
        y_item = item.get(0, 1);
        z_item = item.get(0, 2);
    }
    for(int i = 0; i < tree.numRows(); i++)
    {
        // Getting the x and y coordinates of the tree item.
        if(tree.numCols() == 3)
        {
            x_tree = tree.get(i, 1);
            y_tree = tree.get(i, 2);
            // Finding the euclidean distance of the sample item from the tree item.
            dist = sqrt(((y_tree - y_item) * (y_tree - y_item)) + ((x_tree - x_item) * (x_tree - x_item)));
            // Finding if the distance is nearest.
            if(best > dist)
            {
                best = dist;
                bestrow = i;
            }
        }
        else
        {
            x_tree = tree.get(i, 1);
            y_tree = tree.get(i, 2);
            z_tree = tree.get(i, 3);
            dist = sqrt(((z_tree - z_item) * (z_tree - z_item)) + ((y_tree - y_item) * (y_tree - y_item)) + ((x_tree - x_item) * (x_tree - x_item)));
            if(best > dist)
            {
                best = dist;
                bestrow = i;
            }
        }
        // cout << dist << endl;
    }

    nearestAux(tree, item, 0, tree.numRows()-1, 1, best, bestrow);
    // Returning the tree item number which has minimum distance from the sample item.
    return bestrow;
}



int main()
{
    Matrix items("item");      // the data item to be searched for
    Matrix tree("kdtree");     // a kdtree as a matrix
    SymbolNumMap labels;       // labels for a row labeled matrix (see the matrix library)
    int closest;               // the row that is closest to item using the measure myDist

    // read in labeled training data
    labels = tree.readLabeledRow();

    // build tree
    build(tree, 0, tree.numRows()-1, 1);

    // display tree for comparison in tests
    printf("KDTree version of matrix");
    printf("\n");
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

        // use kdtree to find row number of closest in data
        closest = nearest(tree, *item);

        printf("ANS: ");
        printf("%d %s ", closest, labels.getStr(tree.get(closest, 0)).c_str());
        tree.subMatrix(closest, 1, 1, 0).writeLine(0);
        printf("\n");
    }

    return 0;
}
