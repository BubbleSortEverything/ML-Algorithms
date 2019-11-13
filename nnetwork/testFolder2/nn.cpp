#include "mat.h"
#include "rand.h"

using namespace std;

// Function Prototypes
double biasFun(double x);
double transferFun(double x);
double stepFun(double x);
double oneMinusFun(double x);



int main(int argc, char ** argv){    

    freopen("testsine.mat","r", stdin);    // Used for debugging for getting stdin from file

    int steps, stride, hidden, numCols, numRows;
    double eta, momentum;


    scanf("%d", &steps);
    scanf("%d", &stride);
    scanf("%d", &hidden);
    
    Matrix training_Matrix;

    training_Matrix.read();
    
    // Find # of rows and columns in training matrix
    numCols = training_Matrix.numCols();
    numRows = training_Matrix.numRows();

    // Setup matrix of input values for time series
    Matrix input_matrix = training_Matrix.seriesSampleCol(0, steps, stride);
    input_matrix.normalize();

    // Setup target values matrix for input_matrix
    Matrix target_values;
    target_values = input_matrix.extract(0, steps, input_matrix.numRows(), input_matrix.numCols() - steps);

    // Setup training values with bias matrix copying training value matrix 
    // and adding a column contain bias input

    Matrix input_w_bias = Matrix(input_matrix.numRows(), input_matrix.numCols(), "input with bias");
    input_w_bias.insert(input_matrix, 0, 0);
    input_w_bias.constantCol(steps, -1.0);
    

    // Setup Weight matrix
    initRand();
    Matrix weight_Matrix(hidden + 1, target_values.numCols(), "weight matrix");
    weight_Matrix.rand(-0.25, 0.25);

    // Setup V Matrix
    Matrix v_Matrix(input_matrix.numCols(), hidden, "hidden matrix");
    v_Matrix.rand(-0.25, 0.25);

    // Create Update Matrices
    Matrix hUpdate(v_Matrix, "Hidden Update");
    hUpdate.constant(0.0);

    Matrix wUpdate(weight_Matrix, "Weight Update");
    wUpdate.constant(0.0);


    // Loop for learning
    eta = 0.004;
    momentum = 0.9;
    
    for(int i = 0; i < 30000; i++)
    {
        //          Forwards
        
        
        // Create temporary matrix with target values
        Matrix temp_target_values(target_values);
        // Create h matrix with output of first layer dotted with inputs
        Matrix h = input_w_bias.dot(v_Matrix);
        h.map(&transferFun);
        // Create h matrix with bias
        Matrix h_bias(h.numRows(), h.numCols() + 1);
        h_bias.insert(h, 0, 0);
        h_bias.constantCol(h_bias.numCols() - 1, -1.0);
        // Create y matrix with output of h matrix dotted with weight matrix
        Matrix output = h_bias.dot(weight_Matrix);
        

        //          Backwards

        // Create dy matrix with output matrix and target values
        Matrix dy(output,"delta y");
        dy.sub(temp_target_values);
        dy.scalarMul(1 / double(input_w_bias.numRows()));

        // Create dh_bias matrix with dy, hb, and weight matrix
        Matrix dh_bias = Matrix(h_bias);
        Matrix one_minus_h_bias(h_bias, "Hidden Bia Minus One");
        one_minus_h_bias.map(&oneMinusFun);
        Matrix weight_transposed = weight_Matrix.transpose();
        Matrix dy_dot_tWeight = dy.dot(weight_transposed);
        dh_bias.mul(one_minus_h_bias);
        dh_bias.mul(dy_dot_tWeight);

        //          Update

        // Update weight matrix
        Matrix Th_bias_dot_dy = h_bias.Tdot(dy);
        Th_bias_dot_dy.scalarMul(eta);
        wUpdate.scalarMul(momentum);
        Th_bias_dot_dy.add(wUpdate);
        weight_Matrix.sub(Th_bias_dot_dy);
        wUpdate = Matrix(Th_bias_dot_dy);

        // Create dh matrix without bias
        Matrix dh = dh_bias.extract(0, 0, dh_bias.numRows(), dh_bias.numCols() - 1);

        // Update v matrix
        Matrix Txb_dot_dh = input_w_bias.Tdot(dh);
        Txb_dot_dh.scalarMul(eta);
        hUpdate.scalarMul(momentum);
        Txb_dot_dh.add(hUpdate);
        v_Matrix.sub(Txb_dot_dh);
        hUpdate = Matrix(Txb_dot_dh);

    }
      

    // Final Testing

    Matrix H = input_w_bias.dot(v_Matrix);
    H.map(&transferFun);
    Matrix H_bias(H.numRows(), H.numCols() + 1);
    H_bias.insert(H, 0, 0);
    H_bias.constantCol(H_bias.numCols() - 1, -1.0);
    Matrix finalOutput = H_bias.dot(weight_Matrix);

    // Create Comparision Matrices
    Matrix Comparision(target_values.numRows(), target_values.numCols() + 1);
    Comparision.constant(0);
    Comparision.insert(target_values, 0, 1);
    Comparision.insert(finalOutput, 0, 0);
    
    // Final Printing
    input_matrix.printfmt("Sampled Normalized Input");
    Comparision.printfmt("Est. and Target");
    double distance = finalOutput.dist2(target_values);
    printf("Dist: %f\n", distance);

    return 0;
}

double biasFun(double x){
    return -1.0;
}

double transferFun(double x){
    double slope = 1.25;
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

