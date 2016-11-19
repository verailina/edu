#include "CubicEquationSolver.h"
#define _USE_MATH_DEFINES
#include <cmath>

const double kEpsilon = 1e-9;

int CubicEquationSolver::Solve(double a, double b, double c, double d,
                  double &root1, double &root2, double &root3)
{
    if (a == 0)
    {
        return 1;
    }

    if (d == 0)
    {
        return 1;
    }
    b /= a;
    c /= a;
    d /= a;
    double disc, q, r, dum1, s, t, term1, r13;
    q = (3.0 * c - (b * b)) / 9.0;
    r = -(27.0 * d) + b * (9.0 * c - 2.0 * (b * b));
    r /= 54.0;
    disc = q * q * q + r * r;
    root1 = 0;
    term1 = (b / 3.0);
    if (disc > kEpsilon)
    {
        s = r + sqrt(disc);
        s = ((s < 0) ? - pow(-s, (1.0 / 3.0)) : pow(s, (1.0 / 3.0)));
        t = r - sqrt(disc);
        t = ((t < 0) ? - pow(-t, (1.0 / 3.0)) : pow(t, (1.0 / 3.0)));
        root1 = -term1 + s + t;
        return 1;
    }
    if (fabs(disc) < kEpsilon)
    {
        r13 = ((r < 0) ? - pow(-r, (1.0 / 3.0)) :  pow(r, (1.0 / 3.0)));
        root1 = -term1 + 2.0*r13;
        root2 = -(r13 + term1);
        return 2;
    }
    q = -q;
    dum1 = q * q * q;
    dum1 = acos(r / sqrt(dum1));
    r13 = 2.0 * sqrt(q);
    root1 = -term1 + r13 * cos(dum1 / 3.0);
    root2 = -term1 + r13 * cos((dum1 + 2.0 * M_PI) / 3.0);
    root3 = -term1 + r13 * cos((dum1 + 4.0 * M_PI) / 3.0);
    return 3;
}