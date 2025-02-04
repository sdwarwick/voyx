#pragma once

#include <voyx/Header.h>

namespace $$::midi
{
  template<typename T>
  static inline T key(const T freq, const double concertpitch)
  {
    static_assert(std::is_floating_point<T>::value);

    return static_cast<T>(
      (freq > 0) ? 69 + 12 * std::log2(freq / concertpitch) : 0);
  }

  template<typename T>
  static inline T freq(const T key, const double concertpitch)
  {
    static_assert(std::is_floating_point<T>::value);

    return static_cast<T>(
      std::pow(2, (key - 69) / 12) * concertpitch);
  }

  template<typename T>
  static inline std::vector<T> keys(const std::vector<T>& freqs, const double concertpitch)
  {
    static_assert(std::is_floating_point<T>::value);

    std::vector<T> values(freqs.size());
    std::transform(freqs.begin(), freqs.end(), values.begin(),
      [concertpitch](T i) { return $$::midi::key(i, concertpitch); });

    return values;
  }

  template<typename T>
  static inline std::vector<T> keys()
  {
    static_assert(std::is_arithmetic<T>::value);

    std::vector<T> values(128);
    std::iota(values.begin(), values.end(), 0);

    return values;
  }

  template<typename T>
  static inline std::vector<T> freqs(const double concertpitch)
  {
    static_assert(std::is_floating_point<T>::value);

    std::vector<T> values(128);
    std::iota(values.begin(), values.end(), 0);
    std::transform(values.begin(), values.end(), values.begin(),
      [concertpitch](T i) { return $$::midi::freq(i, concertpitch); });

    return values;
  }
}
