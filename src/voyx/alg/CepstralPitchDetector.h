#pragma once

#include <voyx/Header.h>

template<typename T>
class CepstralPitchDetector
{

public:

  CepstralPitchDetector(const std::pair<double, double> roi, const double samplerate) :
    roi(roi),
    samplerate(samplerate)
  {
  }

  double operator()(const voyx::vector<T> cepstrum) const
  {
    const size_t nmin = size_t(0);
    const size_t nmax = cepstrum.size() / 2;

    const size_t qmin = static_cast<size_t>(samplerate / roi.first);
    const size_t qmax = static_cast<size_t>(samplerate / roi.second);

    const size_t imin = std::clamp(std::min(qmin, qmax), nmin, nmax);
    const size_t imax = std::clamp(std::max(qmin, qmax), nmin, nmax);

    voyxassert(imin < imax);

    T value = std::numeric_limits<T>::lowest();
    ptrdiff_t index = -1;

    for (size_t i = imin; i < imax; ++i)
    {
      if (cepstrum[i] > value)
      {
        value = cepstrum[i];
        index = i;
      }
    }

    return index ? samplerate / index : 0;
  }

private:

  const std::pair<double, double> roi;
  const double samplerate;

};
