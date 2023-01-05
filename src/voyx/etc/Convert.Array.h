#pragma once

#include <voyx/Header.h>

namespace $$
{
  template<typename E>
  auto db(E&& expression, const double bias = 1e-7)
  {
    return 20 * xt::log10(E{expression} + bias);
  }

  template<typename E>
  auto a_weighting(E&& expression)
  {
    auto&& f1 = xt::eval(expression);

    auto f2 = xt::pow(f1, 2.0);
    auto f4 = xt::pow(f2, 2.0);

    auto a = f4 * std::pow(12194.0, 2);
    auto b = f2 + std::pow(20.6, 2);
    auto c = f2 + std::pow(107.7, 2);
    auto d = f2 + std::pow(737.9, 2);
    auto e = f2 + std::pow(12194.0, 2);

    return xt::eval(a / (b * xt::sqrt(c * d) * e));
  }

  template<typename E>
  auto a_weighting_db(E&& expression)
  {
    return xt::eval($$::db($$::a_weighting(expression)) + 2);
  }

  template<typename E>
  std::vector<size_t> findpeaks(E&& expression, const size_t radius = 0)
  {
    auto&& array = xt::eval(expression);

    if (!array.size())
    {
      return {};
    }

    std::vector<size_t> peaks;

    if (radius)
    {
      const ptrdiff_t r = radius;

      for (ptrdiff_t i = r; i < array.size() - r; ++i)
      {
        const auto value = array.flat(i);

        bool ispeak = true;

        for (ptrdiff_t j = i - r; j <= i + r; ++j)
        {
          if (j == i)
          {
            continue;
          }

          if (array.flat(j) > value)
          {
            ispeak = false;
            break;
          }
        }

        if (ispeak)
        {
          peaks.push_back(i);
        }
      }
    }
    else
    {
      auto value = array.flat(0);
      size_t index = 0;

      for (size_t i = 1; i < array.size(); ++i)
      {
        if (array.flat(i) > value)
        {
          value = array.flat(i);
          index = i;
        }
      }

      peaks.push_back(index);
    }

    return peaks;
  }

  // TODO: use xtensor

  template<typename value_getter_t, typename T>
  std::vector<size_t> argmax(const voyx::matrix<T> matrix, const size_t axis = 0)
  {
    using value_t = typename $$::typeofvalue<T>::type;
    const value_getter_t getvalue;

    static_assert(std::is_arithmetic<value_t>::value);

    std::vector<size_t> indices;

    if (matrix.empty())
    {
      return indices;
    }

    const size_t shape[] =
    {
      matrix.size(),
      matrix.stride()
    };

    if (axis == 0)
    {
      indices.resize(shape[1]);

      for (size_t i = 0; i < shape[1]; ++i)
      {
        value_t value = getvalue(matrix(0, i));
        size_t index = 0;

        for (size_t j = 1; j < shape[0]; ++j)
        {
          if (getvalue(matrix(j, i)) > value)
          {
            value = getvalue(matrix(j, i));
            index = j;
          }
        }

        indices[i] = index;
      }
    }
    else if (axis == 1)
    {
      indices.resize(shape[0]);

      for (size_t i = 0; i < shape[0]; ++i)
      {
        value_t value = getvalue(matrix(i, 0));
        size_t index = 0;

        for (size_t j = 1; j < shape[1]; ++j)
        {
          if (getvalue(matrix(i, j)) > value)
          {
            value = getvalue(matrix(i, j));
            index = j;
          }
        }

        indices[i] = index;
      }
    }
    else
    {
      throw std::runtime_error("Invalid axis index!");
    }

    return indices;
  }
}
