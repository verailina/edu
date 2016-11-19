#include <vector>
#include <assert.h>

template <typename Type> void Swap(Type first_element, Type second_element)
{
    auto temp_element = *first_element;
    *first_element = *second_element;
    *second_element = temp_element;
}

template <typename Iterator> void Reverse(Iterator begin, Iterator end)
{
    Iterator forward_iterator = begin;
    Iterator backward_iterator = end - 1;
    while (forward_iterator < backward_iterator)
    {
        Swap<Iterator>(forward_iterator, backward_iterator);
        ++forward_iterator;
        --backward_iterator;
    }
}

template <typename Iterator> void Rotate(Iterator begin, Iterator end, int shift)
{
    int vector_size = end - begin;
    shift %= vector_size;
    Iterator cut_pointer = (shift > 0) ? begin + shift : end + shift;
    Reverse(begin, cut_pointer);
    Reverse(cut_pointer, end);
    Reverse(begin, end);
}

void RotateTest(const std::vector<int> &expected_result, int shift)
{
    int test_array[] = {1, 2, 3, 4, 5};
    int test_array_size = 5;
    std::vector<int> test_vector(test_array, test_array + test_array_size);

    Rotate<int *>(test_array, test_array + test_array_size, shift);
    Rotate<std::vector<int>::iterator>(test_vector.begin(), test_vector.end(), shift);

    int expected_result_size = expected_result.size();
    assert(test_vector.size() == expected_result_size);
    assert(test_array_size == expected_result_size);

    for (int i = 0; i < expected_result_size; i++)
    {
        assert(test_array[i] == expected_result[i]);
        assert(test_vector[i] == expected_result[i]);
    }
}

int main()
{
    RotateTest({3, 4, 5, 1, 2}, 2);
    RotateTest({4, 5, 1, 2, 3}, -2);
    RotateTest({1, 2, 3, 4, 5}, 0);
    RotateTest({1, 2, 3, 4, 5}, 5);
    RotateTest({4, 5, 1, 2, 3}, 8);
    RotateTest({3, 4, 5, 1, 2}, -8);
}