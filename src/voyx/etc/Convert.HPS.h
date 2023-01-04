#pragma once

#include <voyx/Header.h>

// http://musicweb.ucsd.edu/~trsmyth/analysis/Harmonic_Product_Spectrum.html

namespace $$
{
  template<typename T>
  std::vector<T> hpsmul(const std::vector<T>& vector, const size_t depth, const T empty = 0)
  {
    std::vector<T> product(vector.begin(), vector.end());

    for (size_t step = 2; step < depth + 2; ++step)
    {
      std::vector<T> harmonic(vector.size(), empty);

      for (size_t i = 0, j = 0; i < vector.size(); i += step, j += 1)
      {
        harmonic[j] = vector[i];
      }

      for (size_t i = 0; i < vector.size(); ++i)
      {
        product[i] /* multiply */ *= harmonic[i];
      }
    }

    return product;
  }

  template<typename T>
  std::vector<T> hpsadd(const std::vector<T>& vector, const size_t depth, const T empty = -120)
  {
    std::vector<T> product(vector.begin(), vector.end());

    for (size_t step = 2; step < depth + 2; ++step)
    {
      std::vector<T> harmonic(vector.size(), empty);

      for (size_t i = 0, j = 0; i < vector.size(); i += step, j += 1)
      {
        harmonic[j] = vector[i];
      }

      for (size_t i = 0; i < vector.size(); ++i)
      {
        product[i] /* add */ += harmonic[i];
      }
    }

    return product;
  }
}
