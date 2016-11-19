#include <list>
#include <vector>
#include <iterator>
#include <cassert>
#include <string>
#include <set>

template <typename IteratorType, typename ValueType> 
bool binary_search(IteratorType begin, IteratorType end, const ValueType& value)
{
    std::iterator_traits<IteratorType>::iterator_category category;
    return binary_search(begin, end, value, category);
}

template <typename IteratorType, typename ValueType>
bool binary_search(
    IteratorType begin,
    IteratorType end,
    const ValueType& value,
    std::random_access_iterator_tag)
{
    IteratorType left_bound = begin;
    IteratorType right_bound = end;
    int distance = right_bound - left_bound;
    while (distance > 0)
    {
        IteratorType middle = left_bound + distance / 2;
        if (*middle < value)
        {
            left_bound = ++middle;
        }
        else
        {
            right_bound = middle;
        }
        distance = right_bound - left_bound;
    }
    return left_bound != end && !(value < *begin);
}

template <typename IteratorType, typename ValueType>
bool binary_search(
    IteratorType begin,
    IteratorType end,
    const ValueType& value,
    std::forward_iterator_tag)
{
    for (IteratorType iterator = begin; 
        iterator != end; 
        ++iterator)
    {
        if (*iterator == value)
        {
            return true;
        }
    }
    return false;
}

int main()
{
    int array[5] = { 1, 1, 2, 1, 1 };
    std::vector<int> vector = { 1, 2, 3, 3, 4, 5 };
    std::vector<std::string> vector_string = { "aa", "aab", "aba", "abc" };
    std::list<int> list = { 1, 2, 3, 4 };
    std::set<int> set = { 1, 2, 3, 4 };

    assert(binary_search(vector.begin(), vector.end(), 6) == false);
    assert(binary_search(vector.begin(), vector.end(), 3) == true);
    assert(binary_search(vector_string.begin(), vector_string.end(), "abc") == true);

    assert(binary_search(array, array + 5, 1) == true);
    assert(binary_search(array + 3, array + 5, 2) == false);

    assert(binary_search(list.begin(), list.end(), 6) == false);
    assert(binary_search(list.begin(), list.end(), 1) == true);

    assert(binary_search(set.begin(), set.end(), 3) == true);
    assert(binary_search(set.begin(), set.end(), 30) == false);

    return 0;
}