#pragma once
#include "Image.h"
#include <vector>
#include "Analyzer.h"

class RSMethod : public Analyzer
{
public:
    RSMethod(const std::vector<int> &mask);
    double Analyze(const Image &image);

private:
    std::vector<int> mask_;
    Image image_;

    double Solve(double a, double b, double c);
    std::vector<int> GetNegativeMask();
    void CountGroups(int &regular_groups, int &singular_groups, const std::vector<int> &mask);
    int CalculateF(const std::vector<BYTE> &group);

    std::vector<BYTE> GetGroup(int start_pixel);
    std::vector<BYTE> GetGroupF(int start_pixel, const std::vector<int> &mask);
    BYTE GetFPositivePixel(BYTE i);
    BYTE GetFNegativePixel(BYTE i);

    int regular_groups_ = 0;
    int singular_groups_ = 0;

    int regular_groups_negative_ = 0;
    int singular_groups_negative_ = 0;

    int regular_groups_invert_ = 0;
    int singular_groups_invert_ = 0;

    int regular_groups_negative_invert_ = 0;
    int singular_groups_negative_invert_ = 0;
};

