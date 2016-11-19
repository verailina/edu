#include "WSMethod.h"


WSMethod::WSMethod()
{
}

double WSMethod::Analyze(const Image &image)
{
    WSFilter filter;
    filter.Learn(image);
    //filter.SetSimpleFilter(image);

    double message_length = 0;
    int image_size = image.height() * image.width();
    double total_weight = 0;
    for (int i = 1; i < image.height() - 1; ++i)
    {
        for (int j = 1; j < image.width() - 1; ++j)
        {
            double pixel = image.get(i, j);
            double pixel_inverted = image.get(i, j) ^ 1;
            double pixel_predicted = filter.Predict(image, i, j);
            double weight = CountWeight(image, i, j);
            total_weight += weight;
            double prediction_error = pixel - pixel_predicted;
            double inverted_dif = pixel - pixel_inverted;
            message_length += weight * (prediction_error * inverted_dif);
        }
    }
    double normalizing_coefficient = 2.0 / total_weight;
    // double normalizing_coefficient = 2.0 / (image.width() - 2) * (image.height() - 2);
    message_length *= normalizing_coefficient;
    return message_length;
}



double WSMethod::CountWeight(const Image &image, int i, int j)
{
    double exp = 0;
    double exp2 = 0;

    for (int r = -1; r <= 1; ++r)
    {
        for (int l = -1; l <= 1; ++l)
        {
            if (r == 0 && l == 0)
            {
                continue;
            }
            exp += image.get(i + r, j + l);
            exp2 += pow(image.get(i + r, j + l), 2);
        }
    }
    double local_variance = (exp2 - pow(exp, 2) / 8.0) / 7.0;

    return 1.0 / (5.0 + pow(local_variance, 2));
}

double WSMethod::CountWeight2(const Image &image, int i, int j)
{
    int image_size = image.height() * image.width();
    double average = 0;
    double variance = 0;

    for (int r = -1; r <= 1; ++r)
    {
        for (int l = -1; l <= 1; ++l)
        {
            if (r == 0 && l == 0)
            {
                continue;
            }
            average += image.get(i + r, j + l);
        }
    }
    average /= 8;

    for (int r = -1; r <= 1; ++r)
    {
        for (int l = -1; l <= 1; ++l)
        {
            if (r == 0 && l == 0)
            {
                continue;
            }
            double pixel = image.get(i + r, j + l);
            variance += pow(pixel - average, 2);
        }
    }
    variance /= 7;

    return 1.0 / (5.0 + pow(variance, 2));
}