#ifndef EXAMPLES_SRC_INTERP1D_INTERP1D_HPP_
#define EXAMPLES_SRC_INTERP1D_INTERP1D_HPP_

#include <algorithm>
#include <array>
#include <exception>
#include <functional>
#include <iterator>
#include <limits>
#include <type_traits>

template <class RAIterator,
          class Key,
          class ExtractKey,
          class ExtractValue,
          class CompareKey = std::less<Key>>
auto
interp1D(RAIterator const &  begin,
         RAIterator const &  end,
         Key const &         keyVal,
         ExtractKey const &  extractKey,
         ExtractValue const &extractValue,
         CompareKey const &  comp = std::less<Key>())
{
  // I avoid users using wrong iterators
  // I nice use of iterator_traits and iterator_tags
  using category = typename std::iterator_traits<RAIterator>::iterator_category;

  static_assert(std::is_same_v<category, std::bidirectional_iterator_tag> ||
                  std::is_same_v<category, std::random_access_iterator_tag>,
                "Iterators must be (at least) bidirectional");

  // I need at least two point for interpolating anything. This
  // checks also that end is after begin!
  if (std::distance(begin, end) < 1)
    throw std::runtime_error(
      "Interp1D: I need at least 2 points to interpolate!");

  //1) find the interval containing keyval -> [a,b]
  // -> bisection
  RAIterator a{begin};
  RAIterator b{end};

  for (auto dis = std::distance(a,b); dis > 1;)
  {
    RAIterator c = std::next(a, dis/2); // advance a to the mid point [a,b]
    if (comp(keyVal, extractKey(*c)))//(keyVal is in [a,c])
      b = c;
    else
      a = c;

    dis = std::distance(a,b);
  }

  // keyval is in  [a,b]=[a,a+1]
  b = std::next(a,1);
  //what is is a is the last element?
  if(b == end)
  {
    b = a;
    std::advance(a, -1);
  }
  //2) evaluate f(keyVal) al a linea combination of f(a) and f(b).

  const auto key_a = extractKey(*a);
  const auto val_a = extractValue(*a);
  const auto key_b = extractKey(*b);
  const auto val_b = extractValue(*b);

  const auto coeff = (key_b- keyVal) / (key_b - key_a);

  return val_a * coeff +val_b*(1.0 - coeff);
}

#endif /* EXAMPLES_SRC_INTERP1D_INTERP1D_HPP_ */
