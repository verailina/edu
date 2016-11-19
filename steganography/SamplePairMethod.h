#pragma once
#include "Image.h"
#include "Analyzer.h"

class SamplePairMethod : public Analyzer
{
public:
    SamplePairMethod();
    double Analyze(const Image &image);
private:

    int c_m = 0;
    int c_m_1 = 0;
    int e_2m_2 = 0;
    int e_2m = 0;
    int e_2m_1 = 0;

    int o_2m = 0;
    int o_2m_1 = 0;
    int o_2m_2 = 0;

    double CalculateMessageLength(long long a, long long b, long long c);

    bool TestEstimate(double p, double A, double B, double C);
    void RecountSetsCardinalities(int m, const Image &image);
    bool IsEvenPair(int m, BYTE x1, BYTE x2);
    bool IsOddPair(int m, BYTE x1, BYTE x2);
    bool IsTracePair(int m, BYTE x1, BYTE x2);

    void CheckPair(int m, BYTE x1, BYTE x2);
    void SetZeroValues();
    BYTE GetPixelValue(int i, int j, const Image &image);
};

