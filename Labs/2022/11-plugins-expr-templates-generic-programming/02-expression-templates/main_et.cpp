#include <cassert>
#include <iostream>
#include <vector>

// We want an automatic way to convert result = x + x + y * y (that generates temporaries object) in the for loop (which is much more optimal)

template <typename T, typename Container = std::vector<T>>
class MyVector
{
public:
  // MyVector with initial size.
  MyVector(const size_t &n)
    : cont(n)
  {}

  // MyVector with initial size and value.
  MyVector(const size_t &n, const double &initial_value)
    : cont(n, initial_value)
  {}

  // Constructor for underlying container.
  MyVector(const Container &other)
    : cont(other)
  {}

  // Assignment operator for MyVector of different type.
  template <typename T2, typename R2>
  MyVector &
  operator=(const MyVector<T2, R2> &other)
  {
    assert(size() == other.size());

    for (size_t i = 0; i < cont.size(); ++i)
      cont[i] = other[i];

    return (*this);
  }

  size_t
  size() const
  {
    return cont.size();
  }

  T
  operator[](const size_t &i) const
  {
    return cont[i];
  }

  T &
  operator[](const size_t &i)
  {
    return cont[i];
  }

  const Container &
  data() const
  {
    return cont;
  }

  Container &
  data()
  {
    return cont;
  }

private:
  Container cont;
};

// A binary operator for a + b (a,b can be different type a-Op1; b-Op2)
template<class T, class Op1, class Op2>
class MyVectorAdd
{
public:
  MyVectorAdd(const Op1 &a_, const Op2 &b_)
      :a(a_), b(b_)
  {
    assert(a.size() == b.size());
  }

  T
  operator[](const size_t &i)
  {
    return a[i]+b[i];
  }

  size_t
  size()
  {
    return a.size();
  }


private:
  const Op1 &a;
  const Op2 &b;

}

template<class T, class Op1, class Op2>
class MyVectorMul
{
public:
  MyVectorAdd(const Op1 &a_, const Op2 &b_)
      :a(a_), b(b_)
  {
    assert(a.size() == b.size());
  }

  T
  operator[](const size_t &i)
  {
    return a[i]*b[i];
  }

  size_t
  size()
  {
    return a.size();
  }


private:
  const Op1 &a;
  const Op2 &b;

}


template<class T, class V1, class V2>
MyVector<T, MyVectorAdd<T,V1,V2>>
operator+(const MyVector<T, V1> &a, const MyVector<T,V2> &b)
{
  return MyVector<T, MyVectorAdd<T,V1,V2>>(MyVectorAdd<T,V1,V2>(a.data(), b.data()));
}

operator*(MyVector<> a, MyVector<> b)
{

}

// A program that evaluates (x + x + y * y).
int
main(int argc, char **argv)
{
  constexpr size_t N = 1e8;

  MyVector<double> x(N, 5.4);
  MyVector<double> y(N, 10.3);

  // Compute x + x + y * y.

  return 0;
}
