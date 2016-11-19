#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

const int MAX_COORDINATE = 1000;
const double ACCURACY = 1e-5;

class Bound
{
public:

    enum BoundType
    {
        OPEN,
        CLOSE
    };
    
    Bound(double coordinate_x, BoundType type)
    {
        this->coordinate_x = coordinate_x;
        this->type = type;
    }

    bool operator < (const Bound &another) const
    {
        return this->coordinate_x < another.coordinate_x;
    }

    bool operator == (const Bound &another) const
    {
        return !(*this < another) && !(another < *this);
    }

    bool IsOpen() const
    {
        return type == OPEN;
    }

private:
    double coordinate_x;
    BoundType type;
};

struct Point
{
    int coordinate_x;
    int coordinate_y;
};

typedef std::vector<Bound>::iterator Iterator;

class QuickSort
{
public:
    static void Sort(const Iterator &begin, const Iterator &end)
    {
        if (end - begin < 2)
        {
            return;
        }
        Iterator pivot = ChoosePivot(begin, end);
        Iterator right_half_first = LomutoPartition(begin, end, pivot);
        Sort(begin, right_half_first);
        Sort(right_half_first, end);
    }

private:
    static Iterator ChoosePivot(const Iterator &begin, const Iterator &end)
    {
        int size = end - begin;
        return begin + rand() % size;
    }
    
    static Iterator LomutoPartition(const Iterator &begin,
                                    const Iterator &end, 
                                    const Iterator &pivot_it)
    {
        Bound pivot = *pivot_it;
        int left_half_last = -1;

        for (Iterator right_half_iterator = begin;
            right_half_iterator != end;
            ++right_half_iterator)
        {
            const int left_part_size = left_half_last + 1;
            const int right_part_size = right_half_iterator - begin - left_half_last;

            if (*right_half_iterator < pivot ||
                (*right_half_iterator == pivot && 
                left_part_size < right_part_size))
            {
                left_half_last++;
                std::swap(*(begin + left_half_last), *right_half_iterator);
            }
        }
        return begin + left_half_last + 1;
    }
};

std::vector<Bound> GenerateBounds(const std::vector<Point> &points, double radius)
{
    std::vector<Bound> bounds;
    for (int i = 0;  i < points.size(); ++i)
    {        
        double neighbourhood_size_sqr = pow(radius, 2) - pow(points[i].coordinate_y, 2);
        if (neighbourhood_size_sqr >= 0)
        {
            double neighbourhood_size = sqrt(neighbourhood_size_sqr);
            Bound left_bound(points[i].coordinate_x - neighbourhood_size, Bound::OPEN);
            Bound right_bound(points[i].coordinate_x + neighbourhood_size, Bound::CLOSE);
            bounds.push_back(left_bound);
            bounds.push_back(right_bound);
        }
    }
    QuickSort::Sort(bounds.begin(), bounds.end());
    return bounds;
}

int GetNumberPointsInCircle(double radius, const std::vector<Point> &points)
{
    std::vector<Bound> bounds = GenerateBounds(points, radius);
    int number_points_in_circle = 0;
    int current_points_number = 0;
    for (int index = 0; index < bounds.size(); ++index)
    {
        if (bounds[index].IsOpen())
        {
            current_points_number++;
        }
        else
        {
            current_points_number--;
        }        
        number_points_in_circle = std::max(number_points_in_circle, current_points_number);
    }
    return number_points_in_circle;
}

double FindMinimalRadius(
    double accuracy, 
    const std::vector<Point> &points, 
    int points_in_circle)
{
    double left_bound = 0.0;
    double right_bound = sqrt(2) * MAX_COORDINATE;
    while (right_bound - left_bound > accuracy)
    {
        double middle = (left_bound + right_bound) * 0.5;
        if (GetNumberPointsInCircle(middle, points) >= points_in_circle)
        {
            right_bound = middle;
        }
        else
        {
            left_bound = middle;
        }
    }
    return (left_bound + right_bound) * 0.5;
}

void Solve()
{
    std::ios_base::sync_with_stdio(false);
    std::vector<Point> points;

    int points_number;
    std::cin >> points_number;
    int points_in_circle;
    std::cin >> points_in_circle;
    
    for (int i = 0; i < points_number; ++i)
    {
        Point current_point;
        std::cin >> current_point.coordinate_x >> current_point.coordinate_y;
        points.push_back(current_point);
    }
    double min_radius = FindMinimalRadius(ACCURACY, points, points_in_circle);
    std::cout << std::fixed << std::setprecision(6) << min_radius;
}

int main()
{
    Solve();
    return 0;
}

