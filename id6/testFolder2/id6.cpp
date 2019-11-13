#include "id6.h"

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

bool isInMatrix(double val, Matrix m){
    for(int i = 0; i < m.numRows(); i++){
        for(int j = 0; j < m.numCols(); j++){
            if(val == m.get(i,j))
                return 1;
        }
    }
    return 0;
}

Tree *build(const Matrix &data, const Matrix &features, const SymbolNumMap syms, const SymbolNumMap fNames, const Matrix &avail)
{
    double ans;
    Tree *result = NULL;
    int bestFeature = 0;

    printf("================\n");
    data.printStrings(syms);
    avail.transpose().printInt();
    printf("\n");

    // NOTHING LEFT
    if (data.numRows()==0) {                   
        result = new Tree("NONE");  // should NEVER get here
    }

    // IF ONLY ONE TYPE OF ANSWER FOR THIS BRANCH
    else if (isOneLabel(ansCol, data)>=0) {      
        // BUILD LEAF
        ans = data.get(0,ansCol);
        result = new Tree(syms.getStr(ans));
    }

    // IF RUN OUT OF FEATURES TO TEST SO - VOTE
    else if (avail.numRows()==0) {           
        // BUILD LEAF
        bestFeature = vote(data,features);
        result = new Tree(syms.getStrDefault(bestFeature));
    }

    // PICK BEST GAIN AND PROCEED WITH SUBTREES
    else {
        // LOOP THROUGH FEATURE COLUMNS TO FIND BEST GAIN -> bestFeature
        double g = 0.0, bestG = -0.000005;
        for (int i = 0; i < avail.numRows(); ++i)
        {
            g = gain(data, features, avail.get(i, 0));
 
            if(g > bestG){
                bestG = g;
                bestFeature = avail.get(i, 0);
            }
        } 
        int featureToRemove = bestFeature;
        result = new Tree(fNames.getStr(bestFeature));

        // IGNORE FEATURE WITH ONLY ONE FEATURE VALUE PRESENTED
        if (isOneLabel(bestFeature, data)>=0){
            
        }

        // BUILD NODE WITH TWO OR MORE CHILDREN FEATURE VALUES
        else {
            // ADD TWO OR MORE CHILDREN
            int fValue;
            int withoutRow = 0;
            int present = 0;
            Matrix reducedMatrix(avail.numRows() - 1, avail.numCols(), 0.0, "reducedMatrix");
            for(int r = 0; r < avail.numRows(); ++r)
            {
                for(int c = 0; c < avail.numCols(); ++c)
                {
                    fValue = avail.get(r, c);
                    if(fValue != featureToRemove)
                    {
                        reducedMatrix.set(withoutRow, c, fValue);
                        withoutRow++;
                    }
                }
            }
            string nm = data.getName();
            nm = "sub"+nm;
            for(int i = 1; i < maxFeatureValues; i++){
                Matrix cpData = Matrix(data);
                Matrix colData = cpData.extract(0,bestFeature,data.numRows(),1);
                Matrix subData(nm);
                // DO NOT BUILD A CHILD IF THERE IS NO DATA FOR FEATURE VALUE v
                if(isInMatrix(features.get(bestFeature,i),colData)){
                    subData = cpData.subMatrixPickRows(features.get(bestFeature,i),colData);
                    Tree * child = build(subData, features, syms,fNames,reducedMatrix);
                    result->addChild(syms.getStr(features.get(bestFeature,i)) ,child);
                }
                else
                    present = 1;
            }
            // IF NOT ALL FEATURE VALUES REPRESENTED PUT IN DEFAULT AS VOTE FROM DATA
            if(present){
                bestFeature = vote(data,features);
                Tree * child = new Tree(syms.getStrDefault(bestFeature));
                result->addChild("DEFAULT",child);
            }
        }
    }

    return result;  // return here with result from above cascade of if-else
}

string find(Tree *tree, const SymbolNumMap &syms, const SymbolNumMap &fNames, const Matrix &query, int r) {
    // LEAF: THEN RETURN NODE NAME
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
    for (int i=0; i<features.numRows(); i++) 
        fNames.add(syms.getStr(features.get(i, 0)));
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