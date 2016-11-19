#include "RSMethod.h"
#include <cmath>
#include <iostream>

RSMethod::RSMethod(const std::vector<int> &mask)
{
    for (int i = 0; i < mask.size(); ++i)
    {
        mask_.push_back(mask[i] % 2);
    }
}

double RSMethod::Analyze(const Image &image) 
{
    image_ = image;

    std::vector<int> negative_mask = GetNegativeMask();
    int regular_groups = 0;
    int singular_groups = 0;
    CountGroups(regular_groups, singular_groups, mask_);

    
    int regular_groups_negative = 0;
    int singular_groups_negative = 0;
    CountGroups(regular_groups_negative, singular_groups_negative, negative_mask);

    image_.Invert();
    int regular_groups_invert = 0;
    int singular_groups_invert = 0;
    CountGroups(regular_groups_invert, singular_groups_invert, mask_);

    int regular_groups_negative_invert = 0;
    int singular_groups_negative_invert = 0;
    CountGroups(regular_groups_negative_invert, singular_groups_negative_invert, negative_mask);
   
    int total_groups_number = (image.height() * image.width()) / mask_.size();
    double d_0 = (double)(regular_groups - singular_groups) / total_groups_number;
    double d_1 = (double)(regular_groups_invert - singular_groups_invert) / total_groups_number;
    double _d_0 = (double)(regular_groups_negative - singular_groups_negative) / total_groups_number;
    double _d_1 = (double)(regular_groups_negative_invert - singular_groups_negative_invert) / total_groups_number;
    
    return Solve(2 * (d_1 + d_0), (_d_0 - _d_1 - d_1 - 3 * d_0), (d_0 - _d_0));
}

double RSMethod::Solve(double a, double b, double c)
{
    if (a == 0)
    {
        if (b == 0)
        {
            return 0;
        }
        double x = -(double)c / b;
        return x / (x - 0.5);
    }
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return 1;
    }
    double x_1 = (-b + sqrt(discriminant)) / (2 * a);
    double x_2 = (-b - sqrt(discriminant)) / (2 * a);
    double x = (fabs(x_1) < fabs(x_2)) ? x_1 : x_2;
    return x / (x - 0.5);
}


std::vector<int> RSMethod::GetNegativeMask()
{
    std::vector<int> negative_mask;
    for (int i = 0; i < mask_.size(); ++i)
    {
        negative_mask.push_back(-mask_[i]);
    }
    return negative_mask;
}

void RSMethod::CountGroups(int &regular_groups, int &singular_groups, const std::vector<int> &mask)
{
    regular_groups = 0;
    singular_groups = 0;

    int image_size = image_.height() * image_.width();
    for (int i = 0; i < image_.height(); i ++)
        for (int j = 0; j < image_.height() - 4; j += mask.size())
    {
        int number = i * image_.width() + j;
        int f_simple = CalculateF(GetGroup(number));
        int f_mask = CalculateF(GetGroupF(number, mask));
        
        if (f_simple < f_mask)
        {
            regular_groups++;
        }
        if (f_simple > f_mask)
        {
            singular_groups++;
        }
    }
}

int RSMethod::CalculateF(const std::vector<BYTE> &group)
{
    int result = 0;
    for (int i = 0; i < group.size() - 1; ++i)
    {
        result += abs(group[i + 1] - group[i]);
    }
    return result;
}

std::vector<BYTE> RSMethod::GetGroup(int start_pixel)
{
    int mask_size = mask_.size();

    std::vector<BYTE> group;
    for (int j = 0; j < mask_size; ++j)
    {        
         group.push_back(image_.get(start_pixel + j));        
    }

    return group;
}

std::vector<BYTE> RSMethod::GetGroupF(int start_pixel, const std::vector<int> &mask)
{
    int mask_size = mask.size();

    std::vector<BYTE> group_F;
    for (int j = 0; j < mask_size; ++j)
    {
        switch (mask[j])
        {
        case 0:
        {
            group_F.push_back(image_.get(start_pixel + j));
            break;
        }
        case 1:
        {
            group_F.push_back(GetFPositivePixel(image_.get(start_pixel + j)));
            break;

        }
        case -1:
        {
            group_F.push_back(GetFNegativePixel(image_.get(start_pixel + j)));
        }
        }
    }
    
    return group_F;
}

BYTE RSMethod::GetFPositivePixel(BYTE pixel)
{
   return (pixel % 2 == 0) ? pixel + 1 : pixel - 1;    
}

BYTE RSMethod::GetFNegativePixel(BYTE pixel)
{
    return GetFPositivePixel(pixel + 1) - 1;
}

