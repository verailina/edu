#include "SamplePairMethod.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include "CubicEquationSolver.h"

SamplePairMethod::SamplePairMethod()
{
}

double SamplePairMethod::Analyze(const Image &image)
{
    long long A = 0, B = 0, C = 0, D = 0;
    for (int m = -30; m <= 30; ++m)
    {        
        RecountSetsCardinalities(m, image);
        long long a = c_m - c_m_1;
        long long b = 2 * (e_2m_2 + o_2m_2 + 2 * o_2m_1 - e_2m - o_2m - 2 * e_2m_1);
        long long c = 4 * (e_2m_1 - o_2m_1);

        A += a * a;
        B += a * b;
        C += 2 * a * c + b * b;
        D += b * c;
    }
    double root1 = 0, root2 = 0, root3 = 0;
    int roots_number = CubicEquationSolver::Solve(2 * A, 3 * B, C, D, root1, root2, root3);
    
    if (roots_number == 1)
    {
        return root1;
    }

    if (roots_number == 2)
    {
        return std::min(root1, root2);
    }
    return std::min(root1, std::min(root2, root3));
}

bool SamplePairMethod::TestEstimate(double p, double A, double B, double C)
{
    double result = 6 * A * p * p + 6 * B * p + C;
    std::cout << p << " " << result << std::endl;
    return (result > 1e-9) && (((1 - p) * (1 - p)) < 1);
}

void SamplePairMethod::RecountSetsCardinalities(int m, const Image &image)
{
    SetZeroValues();
    int width = image.width();
    int height = image.height(); 
    
    for (int i = 1; i < width - 1; ++i)
    {
        for (int j = 1; j < height - 1; ++j)
        {
            CheckPair(m, GetPixelValue(i, j, image), GetPixelValue(i - 1, j, image));
            CheckPair(m, GetPixelValue(i, j, image), GetPixelValue(i + 1, j, image));
            CheckPair(m, GetPixelValue(i, j, image), GetPixelValue(i, j - 1, image));
            CheckPair(m, GetPixelValue(i, j, image), GetPixelValue(i, j + 1, image));
        }
    }
}

void SamplePairMethod::CheckPair(int m, BYTE x1, BYTE x2)
{
    if (IsTracePair(m, x1, x2)) c_m++;
    if (IsTracePair(m + 1, x1, x2)) c_m_1++;
    if (IsEvenPair(2 * m, x1, x2)) e_2m++;
    if (IsEvenPair(2 * m + 1, x1, x2)) e_2m_1++;
    if (IsEvenPair(2 * m + 2, x1, x2)) e_2m_2++;
    if (IsOddPair(2 * m, x1, x2)) o_2m++;
    if (IsOddPair(2 * m + 1, x1, x2)) o_2m_1++;
    if (IsOddPair(2 * m + 2, x1, x2)) o_2m_2++;
}

void SamplePairMethod::SetZeroValues()
{
    c_m = 0;
    c_m_1 = 0;
    e_2m_2 = 0;
    e_2m = 0;
    e_2m_1 = 0;
    o_2m = 0;
    o_2m_1 = 0;
    o_2m_2 = 0;
}

BYTE SamplePairMethod::GetPixelValue(int i, int j, const Image &image)
{
    int image_width = image.width();
    int image_height = image.height();

    int x_pos = (i + image_width) % image_width;
    int y_pos = (j + image_height) % image_height;

    return image.get(i, j);
}

bool SamplePairMethod::IsEvenPair(int m, BYTE x1, BYTE x2)
{
    return (x1 % 2 == 0) && ((x2 - x1) == m);
}

bool SamplePairMethod::IsOddPair(int m, BYTE x1, BYTE x2)
{
    return (x1 % 2 != 0) && ((x2 - x1) == m);
}

bool SamplePairMethod::IsTracePair(int m, BYTE x1, BYTE x2)
{
    return ((x2 / 2) - (x1 / 2)) == m;
}