#pragma once
#include "Image.h"

class Analyzer
{
public:
    virtual double Analyze(const Image &image) = 0;
};

