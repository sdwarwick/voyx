#pragma once

#include <voyx/Header.h>
#include <voyx/alg/FFT.h>
#include <voyx/alg/Window.h>

namespace $$
{
  template<typename T>
  std::vector<std::complex<T>> fft(const voyx::vector<T> samples, const voyx::vector<T> window)
  {
    voyxassert(samples.size() == window.size());

    FFT<T> fft(samples.size());

    std::vector<T> product(fft.framesize());
    std::vector<std::complex<T>> dft(fft.dftsize());

    for (size_t i = 0; i < fft.framesize(); ++i)
    {
      product[i] = samples[i] * window[i];
    }

    fft.fft(product, dft);

    return dft;
  }

  template<typename T>
  std::vector<std::complex<T>> fft(const voyx::vector<T> samples)
  {
    const std::vector<T> window = Window<T>(samples.size());

    return $$::fft(samples, voyx::vector(window));
  }
}
