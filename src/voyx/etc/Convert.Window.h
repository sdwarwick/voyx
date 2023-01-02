#pragma once

#include <voyx/Header.h>

namespace $$
{
  template<typename T>
  std::vector<T> window(const size_t size)
  {
    std::vector<T> window(size);

    const T pi = T(2) * std::acos(T(-1)) / size;

    for (size_t i = 0; i < size; ++i)
    {
      window[i] = T(0.5) - T(0.5) * std::cos(pi * i);
    }

    return window;
  }
}
