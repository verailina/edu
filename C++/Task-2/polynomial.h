#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <exception>

template <class T>
class Polynomial
{
private:
    std::vector<T> coefficients_;
    static const T kZero;

public:
    typedef typename std::vector<T>::iterator Iterator;
    Polynomial()
    {
    }

    Polynomial(T zero_coefficient)
    {
        coefficients_.push_back(zero_coefficient);
    }

    Polynomial(const std::vector<T> &coefficients)
    {
        coefficients_ = coefficients;
    }

    Polynomial(const Iterator &begin, const Iterator &end)
    {
        coefficients_ = std::vector<T>(begin, end);
    }

    int Degree() const
    {
        int degree = -1;
        const int coefficients_size = coefficients_.size();
        for (int i = coefficients_size - 1; i >= 0; --i)
        {
            if (coefficients_[i] != kZero)
            {
                degree = i;
                break;
            }
        }
        return degree;
    }

    int Degree()
    {
        int degree = ((const Polynomial *)this)->Degree();
        coefficients_.resize(degree + 1);
        return degree;
    }

    Iterator begin()
    {
        return coefficients_.begin();
    }

    Iterator end()
    {
        return coefficients_.end();
    }

    const T &operator[] (int degree_of_coefficient) const
    {
        if (degree_of_coefficient >= coefficients_.size())
        {
            return kZero;
        }
        return coefficients_[degree_of_coefficient];
    }

    T &operator[] (int degree_of_coefficient)
    {
        if (degree_of_coefficient >= coefficients_.size())
        {
            coefficients_.resize(degree_of_coefficient + 1);
        }
        return coefficients_[degree_of_coefficient];
    }

    Polynomial &operator = (const Polynomial &another)
    {
        if (*this != another)
        {
            coefficients_ = another.coefficients_;
        }
        return *this;
    }

    bool operator == (const Polynomial &another) const
    {
        int min_coefficients_size = std::min(coefficients_.size(), another.coefficients_.size());
        for (int i = 0; i < min_coefficients_size; ++i)
        {
            if (coefficients_[i] != another[i])
            {
                return false;
            }
        }
        for (int i = min_coefficients_size; i < coefficients_.size(); ++i)
        {
            if (coefficients_[i] != kZero)
            {
                return false;
            }
        }
        for (int i = min_coefficients_size; i < another.coefficients_.size(); ++i)
        {
            if (another[i] != kZero)
            {
                return false;
            }
        }
        return true;
    }

    bool operator != (const Polynomial &another) const
    {
        return !(*this == another);
    }

    Polynomial &operator += (const Polynomial &another)
    {
        const int coefficients_size = another.coefficients_.size();
        if (coefficients_size > coefficients_.size())
        {
            coefficients_.resize(coefficients_size);
        }
        for (int i = 0; i < coefficients_size; ++i)
        {
            coefficients_[i] += another[i];
        }
        return *this;
    }

    Polynomial &operator += (const T& constant)
    {
        return *this += Polynomial(constant);
    }

    Polynomial &operator -= (const Polynomial &another)
    {
        const int coefficients_size = another.coefficients_.size();
        if (coefficients_size > coefficients_.size())
        {
            coefficients_.resize(coefficients_size);
        }
        for (int i = 0; i < coefficients_size; ++i)
        {
            coefficients_[i] -= another[i];
        }
        return *this;
    }

    Polynomial &operator -= (const T& constant)
    {
        return *this -= Polynomial(constant);
    }

    Polynomial &operator *= (const T &constant)
    {
        const int coefficients_size = coefficients_.size();
        for (int i = 0; i < coefficients_size; ++i)
        {
            coefficients_[i] *= constant;
        }
        return *this;
    }

    Polynomial &operator *= (const Polynomial &another)
    {
        const int this_polynomial_size = coefficients_.size();
        const int another_polynomial_size = another.coefficients_.size();
        std::vector<T> multiplication_result_coefficients(this_polynomial_size + another_polynomial_size + 1);
        for (int i = 0; i < another_polynomial_size; ++i)
        {
            for (int j = 0; j < this_polynomial_size; j++)
            {
                multiplication_result_coefficients[i + j] += another[i] * coefficients_[j];
            }
        }
        coefficients_ = multiplication_result_coefficients;
        return *this;
    }

    T operator() (const T &point) const
    {
        const int polynom_size = coefficients_.size();
        T result = coefficients_[polynom_size - 1];
        for (int i = polynom_size - 2; i >= 0; --i)
        {
            result = coefficients_[i] + result * point;
        }
        return result;
    }
};

template<class T>
const T Polynomial<T>::kZero = T();

template<class T>
Polynomial<T> operator + (const Polynomial<T> &first, const Polynomial<T> &second)
{
    Polynomial<T> addition_result = first;
    addition_result += second;
    return addition_result;
}

template<class T>
Polynomial<T> operator + (const Polynomial<T> &polynomial, const T &constant)
{
    Polynomial<T> addition_result = polynomial;
    addition_result += constant;
    return addition_result;
}

template<class T>
Polynomial<T> operator + (const T &constant, const Polynomial<T> &polynomial)
{
    Polynomial<T> addition_result = polynomial;
    addition_result += constant;
    return addition_result;
}

template<class T>
Polynomial<T> operator - (const Polynomial<T> &first, const Polynomial<T> &second)
{
    Polynomial<T> substruction_result = first;
    substruction_result -= second;
    return substruction_result;
}

template<class T>
Polynomial<T> operator - (const Polynomial<T> &polynomial, const T &constant)
{
    Polynomial<T> substruction_result = polynomial;
    substruction_result -= constant;
    return substruction_result;
}

template<class T>
Polynomial<T> operator - (const T &constant, const Polynomial<T> &polynomial)
{
    Polynomial<T> substruction_result = polynomial * (-1);
    substruction_result += constant;
    return substruction_result;
}
template<class T>
Polynomial<T> operator * (const Polynomial<T> &first, const Polynomial<T> &second)
{
    Polynomial<T> multiplication_result = first;
    multiplication_result *= second;
    return multiplication_result;
}

template<class T>
Polynomial<T> operator * (const Polynomial<T> &polynomial, const T &constant)
{
    Polynomial<T> multiplication_result = polynomial;
    multiplication_result *= constant;
    return multiplication_result;
}

template<class T>
Polynomial<T> operator * (const T &constant, const Polynomial<T> &polynomial)
{
    Polynomial<T> multiplication_result = polynomial;
    multiplication_result *= constant;
    return multiplication_result;
}

template<class T>
Polynomial<T> operator / (const Polynomial<T> &first, const Polynomial<T> &second)
{
    const int first_polynomial_degree = first.Degree();
    const int second_polynomial_degree = second.Degree();
    if (second_polynomial_degree == -1)
    {
        throw std::exception("Division on zero polynomial");
    }

    Polynomial<T> division_result;
    for (int i = first_polynomial_degree; i >= second_polynomial_degree; --i)
    {
        Polynomial<T> rest = first - second * division_result;
        division_result[i - second_polynomial_degree] = rest[i] / second[second_polynomial_degree];
    }
    return division_result;
}

template<class T>
Polynomial<T> operator % (const Polynomial<T> &first, const Polynomial<T> &second)
{
    return first - second * (first / second);
}

template<class T>
Polynomial<T> operator , (const Polynomial<T> &first, const Polynomial<T> &second)
{
    Polynomial<T> zero_polynomial;
    Polynomial<T> first_polynomial = first;
    Polynomial<T> second_polynomial = second;
    while (first_polynomial != zero_polynomial)
    {
        Polynomial<T> temp = first_polynomial;
        first_polynomial = second_polynomial % first_polynomial;
        second_polynomial = temp;
    }
    return second_polynomial;
}

template<class T>
std::ostream & operator << (std::ostream &output_stream, const Polynomial<T> polynomial)
{
    int degree = polynomial.Degree();

    for (int i = 0; i < degree + 1; ++i)
    {
        output_stream << polynomial[i] << " ";
    }
    output_stream << std::endl;
    return output_stream;
}