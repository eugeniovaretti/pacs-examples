#include <dlfcn.h> // contains functionality to parse the ?

#include <cmath>
#include <functional>
#include <iostream>

//nm -a main -> command to denangling. -a returns all the symbols that are defined in the compilation unit.
// _Z3fund hids the fun() -> after the name of the function we have a char that gives info on the type of the param (i -> int)
double
integrand(double x)
{
  return (std::pow(std::sin(std::pow(x, 2)), 2));
}

int
main(int argc, char **argv)
{
  // Load a dynamic (shared) lib containing an implementation of integrate
  // 1)
  //void *handle = dlopen("trapezoidal.so", RTLD_LAZY);
  void *handle = dlopen(argv[1] , RTLD_LAZY); // so know we can call $./main_quadrature adaptive_quadrature.so  (or tapezoidal.so)
  // RTLD_LAZY := all symbols are loaded and evaluated onòy when we call dlsym -> does not throw errors if theare are symbols that are failed to be loaded(_NOW load all the symbols)
  if(handle == nullptr)
  {
    std:: cerr << "Failed to open file" << std::endl;
    return 1;
  }

  // 2) read the symbol
  void *sym = dlsym(handle, "integrate"); // this is the name of function returned with nm with external "C" -> witout it we must had put the complicated zimbol Z9integrate... (ecc..)

  if(sym == nullptr) //(if $./main_quadrature midpoint.so it will raise that error)
  {
    std:: cerr << "Failed to load symbol" << std::endl;
    return 1;
  }

  // 3) convert sym into a callable object
  double (*integrate)(std::function<double(double)>, double, double); //pointer to (function ....)
  integrate = reinterpret_cast<decltype(integrate)>(sym); // because we cannot assign pointers
  const double res = integrate(integrand, 0, M_PI);

  std::cout << "res = " << res << std::endl;

  return 0;
}
