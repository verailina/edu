#pragma once
#include "Analyzer.h"
#include "WSFilter.h"

class WSMethod : public Analyzer
{
public:
    WSMethod();
    double Analyze(const Image &image);

private:
    double CountWeight(const Image &image, int i, int j);
    double CountWeight2(const Image &image, int i, int j);
};

