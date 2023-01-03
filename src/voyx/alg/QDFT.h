#pragma once

#include <voyx/Header.h>

template <typename T, typename F>
class QDFT
{

public:

  QDFT(const double samplerate, const double fmin, const double fmax, const double fnum = 24, const double latency = 0) :
    samplerate(samplerate),
    fmin(fmin),
    fmax(fmax),
    fnum(fnum),
    dftlatency(latency),
    dftquality(std::pow(std::pow(2.0, 1.0 / fnum) - 1.0, -1.0)),
    dftsize(std::ceil(fnum * std::log2(fmax / fmin)))
  {
    const F pi = F(2) * std::acos(F(-1));

    cache.frequencies.resize(dftsize);
    cache.periods.resize(dftsize);
    cache.offsets.resize(dftsize);
    cache.weights.resize(dftsize);
    cache.twiddles.resize(dftsize + 1);

    for (size_t i = 0; i < dftsize; ++i)
    {
      const double frequency = fmin * std::pow(2.0, i / fnum);
      const double period = std::ceil(dftquality * samplerate / frequency);
      const double weight = 1.0 / period;

      cache.frequencies[i] = frequency;
      cache.periods[i] = static_cast<size_t>(period);
      cache.weights[i] = weight;

      const double offset = (cache.periods.front() - period)
        * std::clamp(dftlatency * 0.5 + 0.5, 0.0, 1.0);

      cache.offsets[i] = static_cast<size_t>(offset);

      cache.twiddles[i] = std::polar(F(1), +pi * dftquality / period);
    }

    cache.twiddles.back() = std::polar(F(1), -pi * dftquality);

    buffer.input.resize(cache.periods.front() + 1);
    buffer.output.resize(dftsize);
  }

  size_t size() const
  {
    return dftsize;
  }

  double quality() const
  {
    return dftquality;
  }

  double latency() const
  {
    return dftlatency;
  }

  const std::vector<double>& frequencies() const
  {
    return cache.frequencies;
  }

  void qdft(const T sample, voyx::vector<std::complex<F>> dft)
  {
    std::rotate(buffer.input.begin(), buffer.input.begin() + 1, buffer.input.end());

    buffer.input.back() = sample;

    const std::complex<F> twiddle = cache.twiddles.back();

    for (size_t i = 0; i < dft.size(); ++i)
    {
      const F left = buffer.input[cache.offsets[i] + cache.periods[i]];
      const F right = buffer.input[cache.offsets[i]];

      const std::complex<F> delta = twiddle * left - right;

      dft[i] = buffer.output[i] = cache.twiddles[i] * (buffer.output[i] + delta * cache.weights[i]);
    }
  }

  void qdft(const voyx::vector<T> samples, voyx::matrix<std::complex<F>> dfts)
  {
    for (size_t i = 0; i < samples.size(); ++i)
    {
      qdft(samples[i], dfts[i]);
    }
  }

private:

  const double samplerate;
  const double fmin;
  const double fmax;
  const double fnum;
  const double dftlatency;
  const double dftquality;
  const size_t dftsize;

  struct
  {
    std::vector<double> frequencies;
    std::vector<size_t> periods;
    std::vector<size_t> offsets;
    std::vector<double> weights;
    std::vector<std::complex<F>> twiddles;
  }
  cache;

  struct
  {
    std::vector<T> input;
    std::vector<std::complex<F>> output;
  }
  buffer;

};
