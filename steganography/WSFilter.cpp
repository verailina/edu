#include "WSFilter.h"
#include <boost/numeric/ublas/io.hpp>

const int WSFilter::kFilterSize = 8;

WSFilter::WSFilter()
{
}

void WSFilter::Learn(const Image &image)
{
    int size = (image.height() - 2) * (image.width() - 2);
    Matrix data(size, kFilterSize);
    Vector responses(size);

    int count = 0;
    for (int i = 1; i < image.height() - 1; ++i)
    {
        for (int j = 1; j < image.width() - 1; ++j)
        {
            GetData(data, count, i, j, image);
            responses(count) = image.get(i, j);
            count++;
        }
    }

    coefficients_ = LinearRegression().CountCoefficients(data, responses);
    //std::cout << coefficients_ << std::endl;
}

void WSFilter::SetSimpleFilter(const Image &image)
{
    coefficients_ = Vector(kFilterSize);
    for (int i = 0; i < kFilterSize; ++i)
    {
        coefficients_[i] = 1.0 / kFilterSize;
    }
}

double WSFilter::Predict(const Image &image, int i, int j)
{
    double predicted_pixel = 0;
    int count = 0;
    for (int r = -1; r <= 1; ++r)
    {
        for (int l = -1; l <= 1; ++l)
        {
            if (r == 0 && l == 0)
            {
                continue;
            }
            predicted_pixel += image.get(i + r, j + l) * coefficients_(count++);
        }
    }
    return predicted_pixel;
}

void WSFilter::GetData(Matrix &data, int num, int i, int j, const Image &image)
{
    int count = 0;
    for (int r = -1; r <= 1; ++r)
    {
        for (int l = -1; l <= 1; ++l)
        {
            if (r == 0 && l == 0)
            {
                continue;
            }
            data(num, count) = image.get(i + r, j + l);
            count++;
        }
    }
}
