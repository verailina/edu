#pragma once
#include "LinearRegression.h"
#include "Image.h"

class WSFilter
{
public:
    WSFilter();
    void Learn(const Image &image);
    void SetSimpleFilter(const Image &image);
    double Predict(const Image &image, int i, int j);

private:
    static const int kFilterSize;
    Vector coefficients_;

    void GetData(Matrix &data, int mun, int i, int j, const Image &image);
};

