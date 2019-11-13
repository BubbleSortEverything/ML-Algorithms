/*
  Name: Oshan Karki
  Course: CS475 (Machine Learning)
  Professor: Robert Baker Heckendorn
  Assignment: 7
  Description: A decision tree using ID3 Algorithm implemented in C++. 
*/

/*  Headers from id6.h
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include "tree.h"
#include "mat.h"

// GLOBAL feature stuff
int ansCol;                              // column with answer in it (FOR READABILITY)
int missingValue;                        // num in SymbolNumMap of missingValue string
int maxFeatureValues;                    // the most number of different feature values+1


// // // // // // // // // // // // // // // // // // // // // // // // 
//
//  The answer is always the last row in features and last column in data
//  The data is in two matrices: features and data which looks like:
//  The feature matrix begins with the name of the feature in column 0.
//
// numFeatures maxFeatureValues
// fname numvalues fval1 fval2 fval3...
//  ...
// fname numvalues fval1 fval2 fval3...
// Ans numvalues fval1 fval2 fval3...
// numrows numFeatures
// data data data data data data 
// data data data data data data 
// data data data data data data 
// data data data data data data 

// // // // // // // // // // // // // // // // // // // // // // // // 
//
// ID3 Algorithm
//

// returns the ONE common label as a symbol number if there is one else returns -1
int isOneLabel(int c, const Matrix &d)
{
    double ans;

    ans = d.minCol(c);
    if (ans == d.maxCol(c)) return int(ans);
    else return -1;
}



// given data and features -> entropy in the answer column.
// The less diverse the answers in the set of data lower the entropy.
// The maximum value of entropy is Log2(n) where n is the number of
// different answers in the Ans column of the given data.
double entropy(const Matrix &data, const Matrix &features)
{
    double sum, p;
    int v;

    // look through feature values for feature ansCol starting with col=1
    // since col=0 is the name of the feature
    sum = 0.0;
    for (int i=1; i<maxFeatureValues; i++) {
        v = features.get(ansCol, i);
        if (v==missingValue) break;

        p = data.countEqCol(ansCol, v)/double(data.numRows());
        if (p>0) sum += -p * log2(p);
    }

    return sum;
}


// given data and features and a feature column number -> information gain
double gain(const Matrix &data, const Matrix &features, int fc)
{
    double sum, p;
    int v;

    // look through feature values for feature fc starting with col=1
    // since col=0 is the name of the feature.  Data is subseted by feature fc
    sum = 0.0;
    for (int i=1; i<maxFeatureValues; i++) {
        v = features.get(fc, i);
        if (v==missingValue) break;

        p = data.countEqCol(fc, v)/double(data.numRows());
        if (p>0) sum += p * entropy(data.subMatrixEq(fc, v), features);
    }

    return entropy(data, features) - sum;  // compute the gain
}



// given the data find the most popular answer value searching
// in the order of the feature values.
int vote(const Matrix &data, const Matrix &features) {
    int bestCnt, bestV, cnt;

    printf("VOTE\n");

    // look through all featuresvalue of Ans start with col=1
    bestCnt = -1;
    for (int i=1; i<maxFeatureValues; i++) {
        int v;
        v = features.get(ansCol, i);
        if (v==missingValue) break;

        cnt = data.countEqCol(ansCol, v);
        if (cnt>bestCnt) {
            bestCnt = cnt;
            bestV = v;
        }
    }

    return bestV;
}

/*  function to check if the the value is in matrix
 *  Used to check feature values
 */
bool isInData(double v, Matrix m)
{
    for(int i = 0; i < m.numRows(); i++)
    {
        for(int j = 0; j < m.numCols(); j++)
        {
            if(v == m.get(i,j)){
                return 1;
            }
        }
    }
    return 0;
}

/*  Function to build a tree
 *  I removed the keyword "const" from const avail matrix
 *  so that I could use "operator=(matrix)""
 */
Tree *build(const Matrix &data, const Matrix &features, const SymbolNumMap syms, const SymbolNumMap fNames, Matrix &avail)
{
    double ans;
    Tree *result = NULL;
    int bestFeature = 0;

    // NOTHING LEFT
    if (data.numRows()==0) 
    {                   
        result = new Tree("NONE");  // should NEVER get here
    }

    // IF ONLY ONE TYPE OF ANSWER FOR THIS BRANCH
    // BUILD LEAF
    else if (isOneLabel(ansCol, data)>=0) 
    {      
        ans = data.get(0,ansCol);
        result = new Tree(syms.getStr(ans));
    }

    // IF RUN OUT OF FEATURES TO TEST SO - VOTE
    // BUILD LEAF
    else if (avail.numRows()==0) 
    {           
        int vAns = vote(data,features);     // ------------ Invoke Vote
        result = new Tree(syms.getStrDefault(vAns));
    }

    // PICK BEST GAIN AND PROCEED WITH SUBTREES
    else 
    {
        // LOOP THROUGH FEATURE COLUMNS TO FIND BEST GAIN -> bestFeature
        double currGain = 0.0, bestG = -0.000005;
        for (int i = 0; i < avail.numRows(); ++i)
        {
            currGain = gain(data, features, avail.get(i, 0));
            if(currGain > bestG)
            {
                bestG = currGain;
                bestFeature = avail.get(i, 0);
            }
        } 
        int featureToRemove = bestFeature;

        // IGNORE FEATURE WITH ONLY ONE FEATURE VALUE PRESENTED
        if (isOneLabel(bestFeature, data)>=0)
        {
            bestFeature = vote(data, features);
            result = new Tree(syms.getStr(bestFeature));
        }

        // BUILD NODE WITH TWO OR MORE CHILDREN FEATURE VALUES
        else 
        {
            // ADD TWO OR MORE CHILDREN
            result = new Tree(fNames.getStr(bestFeature));
            int xRow = 0;
            Matrix reducedMatrix(avail.numRows() - 1, avail.numCols(), 0.0, "reducedMatrix");
            for(int r = 0; r < avail.numRows(); ++r)
            {
                for(int c = 0; c < avail.numCols(); ++c)
                {
                    if(avail.get(r, c) != featureToRemove)
                    {
                        reducedMatrix.set(xRow, c, avail.get(r, c));
                        xRow++;
                    }
                }
            }
            string mName = data.getName();
            mName = "sub" + mName;      // -------------- For printing purpose keep adding "sub"
            int numOfChild = 0;
            int numOfFeatures = 0;
            double iVal;

            for(int i = 1; i < maxFeatureValues; i++)
            {
                Matrix cloneData = Matrix(data);
                Matrix columnData = cloneData.extract(0,bestFeature,data.numRows(),1);
                Matrix subData(mName);

                if(features.get(bestFeature, i) != 0.0){
                    numOfFeatures++;
                }

                // DO NOT BUILD A CHILD IF THERE IS NO DATA FOR FEATURE VALUE v
                iVal = features.get(bestFeature, i);
                if(isInData(iVal, columnData))      // ----------------- Check if iVal exists in columnData
                {
                    subData = cloneData.subMatrixPickRows(features.get(bestFeature,i),columnData);
                    Tree * child = build(subData, features, syms,fNames,reducedMatrix);
                    result->addChild(syms.getStr(features.get(bestFeature,i)) ,child);
                    numOfChild++;
                }
            }

            // IF NOT ALL FEATURE VALUES REPRESENTED PUT IN DEFAULT AS VOTE FROM DATA
            if(numOfFeatures != numOfChild)
            {
                bestFeature = vote(data,features);
                Tree * child = new Tree(syms.getStrDefault(bestFeature));
                result->addChild("DEFAULT",child);
            }
        }
    }

    return result;      // --------------- Return calculated result from the build()
}

/*  Recursive Search Function
 */
string find(Tree *tree, const SymbolNumMap &syms, const SymbolNumMap &fNames, const Matrix &query, int r) 
{
    int c;
    Tree *next;
    // LEAF: THEN RETURN NODE NAME
    if(tree->isLeaf())
    {
        return tree->getName();     // --------------------- tree.cpp
    }
    // NOT LEAF
    else 
    {
        // IF EDGE TRY TO FOLLOW EDGE TO SUBTREE
        c = fNames.getNum(tree -> getName());
        if((next = tree -> getChild(syms.getStr(query.get(r, c)))) != NULL)
        {
            return find(next, syms, fNames, query, r);
        }
        else
        {
            next = tree -> getChild("DEFAULT");
            return next -> getName();
        }
    }

    /***********
    ------- ITERATIVE METHOD --------------
    if(tree->isLeaf()){
     return tree->getName();
    }
    // NOT LEAF
    else {
        // IF EDGE TRY TO FOLLOW EDGE TO SUBTREE
        //if(tree->numChildren() > 1){
            Tree * child;
            for(int i = 0; i < query.numCols(); i++){
                child = tree->getChild(syms.getStr(query.get(r,i)));
                if(child != NULL){
                    tree = child;
                }
                // IF NO EDGE FOR FEATURE VALUE THEN GET DEFAULT VALUE
                else{
                    if(tree->isLeaf()){
                        return tree->getName();
                    }
                    child = tree->getChild("DEFAULT");
                }
            }
            return find(child, syms, fNames, query, r);
        //}
    }
    return "ERROR";
     ***************/
}


int main()
{
    Matrix features("Features");
    Matrix data("Data");
    SymbolNumMap syms;
    Tree *tree;

    // READ FEATURES AND DATA
    missingValue = syms.add("-");      // be sure missing values are symbol missingValue.  Save globally
    syms = features.readStrings(syms); // features list in column 0
    syms = data.readStrings(syms);     // each feature in a different column
    data.assertOtherRhs(features, "main");     // check dimensions match
    ansCol = data.numCols()-1;         // NOTE: answer guaranteed in last column!  Save globally
    maxFeatureValues = features.numCols(); // maximum number of feature values anywhere.  Save globally

    // MAKE LIST OF AVAILABLE FEATURES
    Matrix avail(ansCol, 1, "Available");  // a column vector
    avail.initLinear(1, 0, 0);             // list of features

    // MAKE MAPPING Feature names to numbers
    SymbolNumMap fNames("Feature Names");
    for (int i=0; i<features.numRows(); i++){
        fNames.add(syms.getStr(features.get(i, 0)));
    } 

    // BUILD TREE
    tree = build(data, features, syms, fNames, avail);
    tree->printEdge();

    // READ QUERY DATA
    Matrix query("Query");
    query.readStrings(syms);
                                                 
    // FIND AND WRITE QUERY ANSWERS                                                 
    for (int r=0; r<query.numRows(); r++) {
        printf("%d ", r);
        query.writeLineStrings(syms, r);
        printf("%s\n", find(tree, syms, fNames, query, r).c_str());
    }

    return 0;
}