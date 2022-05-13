#ifndef HAVE_TRAPEZOIDAL_HPP
#define HAVE_TRAPEZOIDAL_HPP

#include <functional>

extern "C" // so with nm -a g++ trapezoidal.cpp -c;  $ nm -a trapezoidal.o we have the simple integrate as func name
{
  double
  integrate(std::function<double(double)> f, double a, double b);
}
#endif

// make traspeziodal.cpp <-- (equivalent to) --> g++ -shared trapezoidal.o trapezoidal.so
