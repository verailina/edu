#include <assert.h>

int gcdex(int coefficient_a, int coefficient_b, int & variable_x, int &variable_y)
{
    if (coefficient_a == 0)
    {
        variable_x = 0;
        variable_y = 1;
        return coefficient_b;
    }
    int new_x = 0;
    int new_y = 0;
    int current_gdc = gcdex(coefficient_b % coefficient_a, coefficient_a, new_x, new_y);
    variable_x = new_y - (coefficient_b / coefficient_a) * new_x;
    variable_y = new_x;
    return current_gdc;
}

int inverse(int element, int module)
{
    int inverse_element = 0;
    int coefficient = 0;
    int gcd = gcdex(element, module, inverse_element, coefficient);
    if (gcd != 1)
    {
        return 0;
    }
    return inverse_element;
}

void gcdexTest(int coefficient_a, int coefficient_b, int expected_gcd, int expected_x, int expected_y)
{
    int x = 0;
    int y = 0;
    int gcd = gcdex(coefficient_a, coefficient_b, x, y);
    assert(gcd == expected_gcd);
    assert(x == expected_x);
    assert(y == expected_y);
}

void inverseTest()
{
    assert(inverse(2, 7) == -3);
    assert(inverse(2, 4) == 0);
    assert(inverse(13, 2) == 1);
    assert(inverse(0, 5) == 0);
    assert(inverse(5, 0) == 0);
    assert(inverse(-2, 5) == 2);
}

int main()
{
    gcdexTest(2, 3, 1, -1, 1);
    gcdexTest(6, 9, 3, -1, 1);
    gcdexTest(0, 5, 5, 0, 1);
    gcdexTest(5, -5, 5, 1, 0);

    inverseTest();
    return 0;
}