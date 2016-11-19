#include "LinearRegression.h"
#include "InverseMatrix.h"


LinearRegression::LinearRegression()
{
}

Vector LinearRegression::CountCoefficients(const Matrix &data, const Vector &responses)
{
    Matrix transposed_data = trans(data);
    Matrix squared_data = prod(transposed_data, data);
    Matrix inverse(squared_data.size1(), squared_data.size1());
    InvertMatrix(squared_data, inverse);
    return prod(prod(inverse, transposed_data), responses);
}