#include "polynomial.h"
#include <assert.h>
#include <sstream>

void ConstructionTest()
{
    Polynomial<int> polynomial_0;
    Polynomial<int> polynomial_1(10);
    Polynomial<int> polynomial_2({ 1, 2, 3, 4, 5, 6 });
    Polynomial<int> polynomial_3({ 1, 2, 3, 0, 0, 0 });
    Polynomial<int> polynomial_4(polynomial_2.Lowest(), polynomial_2.Highest() - 3);
}

void ComparisonTest()
{
    assert(Polynomial<int>({ 1, 2, 0, 0 }) == Polynomial<int>({ 1, 2 }));
    assert(Polynomial<int>({ 1, 2 }) == Polynomial<int>({ 1, 2 }));
    assert(Polynomial<int>({ 1, 3 }) != Polynomial<int>({ 1, 2 }));
    assert(Polynomial<int>({ 1, 3 }) != Polynomial<int>({ 1 }));
}

void AdditionTest()
{
    Polynomial<int> polynomial({ 1, 2, 3 });
    polynomial += Polynomial<int>({ 1, 2, 3, 5 });
    assert(polynomial == Polynomial<int>({ 2, 4, 6, 5 }));

    polynomial += Polynomial<int>({ 1, 2 });
    assert(polynomial == Polynomial<int>({ 3, 6, 6, 5 }));

    polynomial += Polynomial<int>({ -1, 0, 0, -5 });
    assert(polynomial == Polynomial<int>({ 2, 6, 6 }));

    polynomial += -2;
    assert(polynomial == Polynomial<int>({ 0, 6, 6 }));
    assert(Polynomial<int>({ 1, 2, 1 }) + 2 == Polynomial<int>({ 3, 2, 1 }));
    assert(0 + Polynomial<int>({ 1, 2, 1 }) == Polynomial<int>({ 1, 2, 1 }));

    assert((Polynomial<int>({ 1, 3 }) + Polynomial<int>({ 1, 3 })) == Polynomial<int>({ 2, 6 }));
    assert((Polynomial<int>({ 1, 3, 2 }) + Polynomial<int>({ 1, 3 })) == Polynomial<int>({ 2, 6, 2 }));
    assert((Polynomial<int>({ 1, 3 }) + Polynomial<int>({ -1, -3 })) == Polynomial<int>());
}

void SubstructionTest()
{
    Polynomial<int> polynomial({ 1, 2, 3, 4, 5 });
    polynomial -= Polynomial<int>({ 1, 2, 3 });
    assert(polynomial == Polynomial<int>({ 0, 0, 0, 4, 5 }));

    polynomial -= 10;
    assert(polynomial == Polynomial<int>({ -10, 0, 0, 4, 5 }));
    assert(Polynomial<int>({ 1, 2, 1 }) - 1 == Polynomial<int>({ 0, 2, 1 }));
    assert(0 - Polynomial<int>({ 1, 2, 1 }) == Polynomial<int>({ -1, -2, -1 }));

    assert((Polynomial<int>({ 1, 3 }) - Polynomial<int>({ 1, 3 })) == Polynomial<int>());
    assert((Polynomial<int>() - Polynomial<int>({ 1, 3, 2 })) == Polynomial<int>({ -1, -3, -2 }));
    assert((Polynomial<int>({ 1, 3, 2, 4 }) - Polynomial<int>({ 0, 0, 0, 4 })) == Polynomial<int>({ 1, 3, 2 }));
}

void MultiplicationTest()
{
    Polynomial<int> polynomial({ 1, 1});
    polynomial *= polynomial;
    assert(polynomial == Polynomial<int>({ 1, 2, 1 }));

    polynomial *= -1;
    assert(polynomial == Polynomial<int>({ -1, -2, -1 }));
    assert(Polynomial<int>({ 1, 2, 1 }) * 5 == Polynomial<int>({ 5, 10, 5 }));
    assert(0 * Polynomial<int>({ 1, 2, 1 }) == Polynomial<int>());

    assert((Polynomial<int>({ 1, 3, 2, 4 }) * Polynomial<int>({ 1 })) == Polynomial<int>({ 1, 3, 2, 4 }));
    assert((Polynomial<int>({ 1, 3, 2, 4 }) * Polynomial<int>({ 0 })) == Polynomial<int>({ 0 }));
    assert((Polynomial<int>({ 1, 3}) * Polynomial<int>({ 1, -1 })) == Polynomial<int>({ 1, 2, -3 }));
    assert((Polynomial<int>({2}) * Polynomial<int>({ 1, 2, 1 })) == Polynomial<int>({ 2, 4, 2 }));
}

void DegreeTest()
{
    Polynomial<int> polynomial;
    assert(polynomial.Degree() == -1);

    polynomial += 1;
    assert(polynomial.Degree() == 0);

    Polynomial<int> polynomial_2({ 1, 2, 3 });
    assert(polynomial_2.Degree() == 2);

    Polynomial<int> polynomial_3({ 1, 2, 3, 0, 0, 0 });
    assert(polynomial_3.Degree() == 2);

    polynomial_2 -= Polynomial<int>({ 1, 2, 3 });
    assert(polynomial_2.Degree() == -1);
}

void IndicesTest()
{
    Polynomial<int> polynomial({ 1, 2, 3 });

    assert(polynomial[0] == 1);
    assert(polynomial[2] == 3);
    assert(polynomial[10] == 0);

    polynomial[100] = -1;
    assert(polynomial[100] == -1);
    assert(polynomial.Degree() == 100);
}

void FunctionTest()
{
    Polynomial<int> polynomial({ 1, 2, 1 });
    assert(polynomial(0) == 1);
    assert(polynomial(10) == 121);
    assert(polynomial(-1) == 0);

    Polynomial<int> polynomial_2({0});
    assert(polynomial_2(10) == 0);

    Polynomial<int> polynomial_3({ 1, -2, 1 });
    assert(polynomial_3(1) == 0);
}

void OutputTest()
{
    Polynomial<int> polynomial({ 1, 2, 3, -4, 5, -5, 0, 0, 0 });
    std::stringstream output_stream;
    output_stream << polynomial;
    assert(output_stream.str() == "1 2 3 -4 5 -5 \n");
}

void IteratorsTest()
{
    Polynomial<int> polynomial({ 1, 2, 3, -4, 5 });

    for (Polynomial<int>::Iterator it = polynomial.Lowest(); it != polynomial.Highest(); ++it)
    {
        (*it)++;
    }
    assert(polynomial == Polynomial<int>({ 2, 3, 4, -3, 6 }));
}

void DivisionTest()
{
    assert((Polynomial<int>({ 1, 3, 3, 1 }) / Polynomial<int> ({ 1, 1 })) == Polynomial<int>({ 1, 2, 1 }));
    assert((Polynomial<int>({ 1, 1 }) / Polynomial<int>({ 1, 3, 3, 1 })) == Polynomial<int>());
    assert((Polynomial<double>({ 1, 3, 3, 1 }) / Polynomial<double>({ 2, 2 })) == Polynomial<double>({ 0.5, 1, 0.5 }));
    assert((Polynomial<double>() / Polynomial<double>({ 2, 2 })) == Polynomial<double>());

    assert((Polynomial<int>({ 3, 2, 1}) % Polynomial<int>({ 1, 1 })) == Polynomial<int>({ 2 }));
    assert((Polynomial<int>({ 1, 3, 3, 1 }) % Polynomial<int>({ 1, 1 })) == Polynomial<int>());

    assert((Polynomial<int>({ 1, 3, 3, 1 }), Polynomial<int>({ 1, 2, 1 })) == Polynomial<int>({ 1, 2,  1 }));
    assert((Polynomial<int>({ 0, 3, 3, 1 }), Polynomial<int>({ 0, 1})) == Polynomial<int>({ 0, 1 }));
    assert((Polynomial<int>({ 1, 3, 3, 1 }), Polynomial<int>({ 0, 1 })) == Polynomial<int>({ 1 }));
}

int main()
{
    ConstructionTest();
    ComparisonTest();
    DegreeTest();

    AdditionTest();
    SubstructionTest();
    MultiplicationTest();

    FunctionTest();
    OutputTest();

    IndicesTest();
    IteratorsTest();

    DivisionTest();
}