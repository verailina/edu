#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <random>

int GCD(int first, int second)
{
    while (first != 0)
    {
        int temp = first;
        first = second % first;
        second = temp;
    }
    return second;
}

int GCD(int first, int second, int third)
{
    return GCD(first, GCD(second, third));
}

class Triangle
{
public:
    static const int kEdgesNumber = 3;

    Triangle(int first_edge, int second_edge, int third_edge)
    {
        edges_.push_back(first_edge);
        edges_.push_back(second_edge);
        edges_.push_back(third_edge);
    }

    int getEdge(int index) const
    {
        return edges_[index];
    }

protected:
    std::vector<int> edges_;
};

class NormalizedTriangle : public Triangle
{
public:
    explicit NormalizedTriangle(const Triangle &triangle) :
        Triangle(triangle)
    {
        std::sort(edges_.begin(), edges_.end());
        int gcd_of_edges = GCD(edges_[0], edges_[1], edges_[2]);
        for (int i = 0; i < kEdgesNumber; ++i)
        {
            edges_[i] /= gcd_of_edges;
        }
    }

    bool operator == (const NormalizedTriangle &another) const
    {
        for (int i = 0; i < kEdgesNumber; ++i)
        {
            if (edges_[i] != another.edges_[i])
            {
                return false;
            }
        }
        return true;
    }
};

class HashTable
{
public:

    explicit HashTable(int table_size) : 
        table_(table_size),
        elements_number_(0)
    {
        std::default_random_engine generator(kSeed);
        std::uniform_int_distribution<int> distribution(0, kPrimeModulo - 1);

        for (int i = 0; i < kRandomCoefficientsNumber; ++i)
        {
            random_coefficients_.push_back(distribution(generator));
        }        
    }

    bool insert(const NormalizedTriangle &triangle)
    {
        int index = hash(triangle);

        for (auto&& hashed_triangle : table_[index])
        {
            if (hashed_triangle == triangle)
            {
                return false;
            }
        }

        table_[index].push_back(triangle);
        elements_number_++;
        return true;
    }

    int getElementsNumber() const
    {
        return elements_number_;
    }

private:

    static const int kPrimeModulo = 1000000007;
    static const int kRandomCoefficientsNumber = Triangle::kEdgesNumber;
    static const int kSeed = 239;

    std::vector<std::list<NormalizedTriangle> > table_;
    std::vector<int> random_coefficients_;
    int elements_number_;

    int hash(const NormalizedTriangle &triangle)
    {
        unsigned long long result = 0;
        for (int index = 0; index < kRandomCoefficientsNumber; ++index)
        {
            result += random_coefficients_[index] * triangle.getEdge(index);
        }

        int hash = (result % kPrimeModulo) % table_.size();
        return hash;
    }
};

int CountTriangleSimilarityClasses(const std::vector<Triangle> &triangles)
{
    int triangles_number = triangles.size();
    HashTable hash_table(triangles_number);

    for (int i = 0; i < triangles_number; ++i)
    {
        hash_table.insert(NormalizedTriangle(triangles[i]));
    }

    return hash_table.getElementsNumber();
}

std::vector<Triangle> ReadTriangles()
{
    std::ios_base::sync_with_stdio(false);
    int triangles_number;
    std::cin >> triangles_number;

    std::vector<Triangle> triangles;
    for (int i = 0; i < triangles_number; ++i)
    {
        int first_edge;
        int second_edge;
        int third_edge;
        std::cin >> first_edge >> second_edge >> third_edge;
        triangles.push_back(Triangle(first_edge, second_edge, third_edge));
    }
    return triangles;
}

int main()
{
    std::vector<Triangle> triangles = ReadTriangles();
    std::cout << CountTriangleSimilarityClasses(triangles) << std::endl;

    return 0;
}
