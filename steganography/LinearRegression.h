#define _SCL_SECURE_NO_WARNINGS
#pragma once
#include "boost\numeric\ublas\matrix.hpp"
#include "boost\numeric\ublas\vector.hpp"

using namespace boost::numeric::ublas;
typedef matrix<double> Matrix;
typedef vector<double> Vector;

class LinearRegression
{
public:
    LinearRegression();
    Vector CountCoefficients(const Matrix &data, const Vector &responses);

};

