#include "trapezoidal.hpp"

#include <cmath>

double
integrate(std::function<double(double)> f, double a, double b)
{
  return 0.5 * (b-a) * (f(a) + f(b));
}
