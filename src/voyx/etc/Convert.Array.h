#pragma once

#include <voyx/Header.h>

namespace $$
{
  template<typename T>
  nc::NdArray<T> ndarray(const T* data, const size_t size)
  {
    return nc::NdArray<T>(data, size);
  }

  template<typename T>
  nc::NdArray<T> ndarray(const std::span<const T> span)
  {
    return nc::NdArray<T>(span.data(), span.size());
  }

  template<typename T>
  nc::NdArray<T> ndarray(const std::span<T> span)
  {
    return nc::NdArray<T>(span.data(), span.size());
  }

  template<typename T>
  nc::NdArray<T> ndarray(const std::vector<T>& vector)
  {
    return nc::NdArray<T>(vector.data(), vector.size());
  }

  template<typename T>
  nc::NdArray<T> ndarray(const voyx::vector<T> vector)
  {
    return nc::NdArray<T>(vector.data(), vector.size());
  }

  template<typename T>
  nc::NdArray<T> ndarray(const voyx::matrix<T> matrix)
  {
    return nc::NdArray<T>(matrix.data(), matrix.size(), matrix.stride());
  }

  template<typename T>
  nc::NdArray<nc::uint32> pickpeaks(const nc::NdArray<T>& vector, const size_t radius = 0)
  {
    if (!vector.size())
    {
      return nc::NdArray<nc::uint32>();
    }

    std::vector<size_t> peaks;

    if (radius)
    {
      const ptrdiff_t r = radius;

      for (ptrdiff_t i = r; i < vector.size() - r; ++i)
      {
        const T value = vector[i];

        bool ispeak = true;

        for (ptrdiff_t j = i - r; j <= i + r; ++j)
        {
          if (j == i)
          {
            continue;
          }

          if (vector[j] > value)
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
      T value = vector[0];
      size_t index = 0;

      for (size_t i = 1; i < vector.size(); ++i)
      {
        if (vector[i] > value)
        {
          value = vector[i];
          index = i;
        }
      }

      peaks.push_back(index);
    }

    auto ndpeaks = nc::NdArray<size_t>(peaks.data(), peaks.size(), false);

    return ndpeaks.astype<nc::uint32>(); // WTF
  }

  // TODO: use numcpp

  template<typename value_getter_t, typename T>
  std::vector<size_t> pickpeaks(const voyx::vector<T> vector, const size_t radius = 0)
  {
    using value_t = typename $$::typeofvalue<T>::type;
    const value_getter_t getvalue;

    static_assert(std::is_arithmetic<value_t>::value);

    std::vector<size_t> peaks;

    if (vector.empty())
    {
      return peaks;
    }

    if (radius)
    {
      const ptrdiff_t r = radius;

      for (ptrdiff_t i = r; i < vector.size() - r; ++i)
      {
        const value_t value = getvalue(vector[i]);

        bool ispeak = true;

        for (ptrdiff_t j = i - r; j <= i + r; ++j)
        {
          if (j == i)
          {
            continue;
          }

          if (getvalue(vector[j]) > value)
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
      value_t value = getvalue(vector[0]);
      size_t index = 0;

      for (size_t i = 1; i < vector.size(); ++i)
      {
        if (getvalue(vector[i]) > value)
        {
          value = getvalue(vector[i]);
          index = i;
        }
      }

      peaks.push_back(index);
    }

    return peaks;
  }

  template<typename value_getter_t, typename T>
  size_t argmax(const voyx::vector<T> vector)
  {
    using value_t = typename $$::typeofvalue<T>::type;
    const value_getter_t getvalue;

    static_assert(std::is_arithmetic<value_t>::value);

    if (vector.empty())
    {
      return -1;
    }

    value_t value = getvalue(vector[0]);
    size_t index = 0;

    for (size_t i = 1; i < vector.size(); ++i)
    {
      if (getvalue(vector[i]) > value)
      {
        value = getvalue(vector[i]);
        index = i;
      }
    }

    return index;
  }

  template<typename value_getter_t, typename T>
  size_t argmax(const std::vector<T>& vector)
  {
    return $$::argmax<value_getter_t>(voyx::vector<T>(vector));
  }

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
