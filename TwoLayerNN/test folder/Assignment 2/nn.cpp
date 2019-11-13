#include "mat.h"
#include "rand.h"

using namespace std;

// Function Prototypes
double biasFun(double x);
double transferFun(double x);
double stepFun(double x);
double oneMinusFun(double x);

int main(int argc, char ** argv){    

    //freopen("newxor.in","r", stdin);    // Used for debugging for getting stdin from file

    int numInputs, numHiddenNodes, ofclasses, numCols, numRows;
    double eta;


    scanf("%d", &numInputs);
    scanf("%d", &numHiddenNodes);
    scanf("%d", &ofclasses);
    
    Matrix training_Matrix;

    training_Matrix.read();
    
    // Find # of rows and columns in training matrix
    numCols = training_Matrix.numCols();
    numRows = training_Matrix.numRows();

    // Setup matrix of target values by extracting target value column(s) from training matrix
    Matrix target_values;
    target_values = training_Matrix.extract(0, numInputs, numRows, numCols - numInputs);

    // Setup training values matrix by extracting columns containing training 
    // values from training matrix
    Matrix x_values;
    x_values = training_Matrix.extract(0, 0, numRows, numCols - target_values.numCols());

    // Setup training values with bias matrix copying training value matrix 
    // and adding a column contain bias input

    Matrix x_bias = new Matrix(numRows, x_values.numCols() + 1, "training values with bias");
    x_bias.insert(x_values, 0, 0);
    x_bias.mapCol(x_bias.numCols() - 1, &biasFun);
    x_bias.normalizeCols();
    x_bias.shuffle();
    //x_bias.print();

    // Setup Weight matrix
    Matrix weight_Matrix = new Matrix(numHiddenNodes + 1, target_values.numCols());
    initRand();
    weight_Matrix.rand(0.0,1.0);
    weight_Matrix.print("weight_matrix");

    // Setup V Matrix
    Matrix v_Matrix = new Matrix(x_bias.numCols(), numHiddenNodes);
    v_Matrix.rand(0.0,1.0);
    v_Matrix.print("v_Matrix");

    // Loop for learning
    eta = 0.05;
    for(int i = 0; i < 20000; i++)
    {
        //          Forwards
        
        // Create temporary matrix with target values
        Matrix temp_target_values = new Matrix(target_values);
        // Create h matrix with output of first layer dotted with inputs
        Matrix h = x_bias.dot(v_Matrix);
        h.map(&transferFun);
        // Create h matrix with bias
        Matrix h_bias = new Matrix(h.numRows(), h.numCols() + 1);
        h_bias.insert(h, 0, 0);
        h_bias.mapCol(h_bias.numCols() - 1, &biasFun);
        // Create y matrix with output of h matrix dotted with weight matrix
        Matrix output = h_bias.dot(weight_Matrix);
        output.map(&transferFun);

        //          Backwards

        // Create dy matrix with output matrix and target values
        Matrix dy = new Matrix(output);
        dy.sub(temp_target_values);
        Matrix one_minus_output = new Matrix(output);
        one_minus_output.map(&oneMinusFun);
        dy.mul(output);
        dy.mul(one_minus_output);

        // Create dh_bias matrix with dy, hb, and weight matrix
        Matrix dh_bias = Matrix(h_bias);
        Matrix one_minus_h_bias = new Matrix(h_bias);
        one_minus_h_bias.map(&oneMinusFun);
        Matrix weight_transposed = weight_Matrix.transpose();
        Matrix dy_dot_tWeight = dy.dot(weight_transposed);
        dh_bias.mul(one_minus_h_bias);
        dh_bias.mul(dy_dot_tWeight);

        //          Update

        // Update weight matrix
        Matrix Th_bias_dot_dy = h_bias.Tdot(dy);
        Th_bias_dot_dy.scalarMul(eta);
        weight_Matrix.sub(Th_bias_dot_dy);

        // Create dh matrix without bias
        Matrix dh = dh_bias.extract(0, 0, dh_bias.numRows(), dh_bias.numCols() - 1);

        // Update v matrix
        Matrix Txb_dot_dh = x_bias.Tdot(dh);
        Txb_dot_dh.scalarMul(eta);
        v_Matrix.sub(Txb_dot_dh);
    }
    

    // Setup Test values matrix
    Matrix test_values;
    test_values.read();
    test_values.print("test_values");

    // Setup test target values matrix
    Matrix test_target;
    test_target = test_values.extract(0, numInputs, test_values.numRows(), test_values.numCols() - numInputs);
    test_target.print("test_target");

    // Setup test input matrix
    Matrix test_input;
    test_input = test_values.extract(0, 0, test_values.numRows(), test_values.numCols() - test_target.numCols());
    test_input.print("test_input");

    // Setup Test input with bias matrix
    Matrix test_with_bias = new Matrix(test_values.numRows(), numInputs + 1, "test values with bias");
    test_with_bias.insert(test_input, 0, 0);
    test_with_bias.mapCol(test_with_bias.numCols() - 1, &biasFun);
    test_with_bias.normalizeCols();    

    // Final Testing

    Matrix h;
    h = test_with_bias.dot(v_Matrix);
    h.map(&transferFun);
    Matrix h_bias = new Matrix(h.numRows(), h.numCols() + 1);
    h_bias.insert(h, 0, 0);
    h_bias.mapCol(h_bias.numCols() - 1, &biasFun);
    Matrix output = h_bias.dot(weight_Matrix);
    output.map(&transferFun);

    // Print target and predicted
    test_target.printfmt("Target","", false);
    output.map(&stepFun);
    output.printfmt("Predicted","", false);
    
    // Setting up the confusion matrices
    Matrix temp_output = new Matrix(output);
    temp_output.map(&stepFun);

    for(int i = 0; i < test_target.numCols() ; i++)
    {
        Matrix confusion_Matrix = new Matrix(ofclasses, ofclasses, "Confusion Matrix");
        confusion_Matrix.constant(0);
        for(int j = 0; j < test_target.numRows(); j++)
        {
            if (test_target.get(j,i) == 1) {
                if (temp_output.get(j,i) == 1) {
                    confusion_Matrix.inc(1,1);
                }
                else{
                    confusion_Matrix.inc(0,1);
                }
            }
            else
            {
                if (temp_output.get(j,i) == 1) {
                    confusion_Matrix.inc(1,0);
                }
                else{
                    confusion_Matrix.inc(0,0);
                }
            }
            
        }
        confusion_Matrix.printfmt("Confusion Matrix","", false);
        
    }
    
    return 0;
}

double biasFun(double x){
    return -1.0;
}

double transferFun(double x){
    double slope = 1;
    return (1.0/(1.0 + exp(-4 * slope * x)));
}

double stepFun(double x){
    if(x > 0.5)
        return 1.0;
    else
        return 0.0;
}

double oneMinusFun(double x){
    return 1 - x;
}