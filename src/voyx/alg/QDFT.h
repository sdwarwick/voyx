#pragma once

#include <voyx/Header.h>

/**
 * Constant Q Sliding DFT implementation according to [1] and [2].
 *
 * [1] Russell Bradford and Richard Dobson and John ffitch
 *     Sliding with a Constant Q
 *     International Conference on Digital Audio Effects (2008)
 *     https://www.dafx.de/paper-archive/2008/papers/dafx08_63.pdf
 *
 * [2] Benjamin Blankertz
 *     The Constant Q Transform
 *     https://doc.ml.tu-berlin.de/bbci/material/publications/Bla_constQ.pdf
 **/
template <typename T, typename F>
class QDFT
{

public:

  QDFT(const double samplerate, std::pair<double, double> roi, const double resolution = 24, const double latency = 0)
  {
    const F pi = F(2) * std::acos(F(-1));

    config.samplerate = samplerate;
    config.roi = roi;
    config.resolution = resolution;
    config.latency = latency;
    config.quality = std::pow(std::pow(2.0, 1.0 / resolution) - 1.0, -1.0);
    config.size = std::ceil(resolution * std::log2(roi.second / roi.first));

    config.frequencies.resize(config.size);
    data.resize(config.size + 1);

    for (size_t i = 0; i < config.size; ++i)
    {
      const double frequency = config.roi.first * std::pow(2.0, i / config.resolution);
      const double period = std::ceil(config.quality * config.samplerate / frequency);
      const double weight = 1.0 / period;

      config.frequencies[i] = frequency;
      data[i].period = static_cast<size_t>(period);
      data[i].weight = weight;

      const double offset = (data.front().period - period)
        * std::clamp(config.latency * 0.5 + 0.5, 0.0, 1.0);

      data[i].offset = static_cast<size_t>(offset);

      data[i].twiddle = std::polar(F(1), +pi * config.quality / period);
    }

    data.back().twiddle = std::polar(F(1), -pi * config.quality);

    buffer.input.resize(data.front().period + 1);
    buffer.output.resize(config.size);
  }

  size_t size() const
  {
    return config.size;
  }

  double quality() const
  {
    return config.quality;
  }

  double latency() const
  {
    return config.latency;
  }

  const std::vector<double>& frequencies() const
  {
    return config.frequencies;
  }

  void qdft(const T sample, voyx::vector<std::complex<F>> dft)
  {
    std::rotate(buffer.input.begin(), buffer.input.begin() + 1, buffer.input.end());

    buffer.input.back() = sample;

    const std::complex<F> twiddle = data.back().twiddle;

    for (size_t i = 0; i < dft.size(); ++i)
    {
      const F left = buffer.input[data[i].offset + data[i].period];
      const F right = buffer.input[data[i].offset];

      const std::complex<F> delta = twiddle * left - right;

      dft[i] = buffer.output[i] = data[i].twiddle * (buffer.output[i] + delta * data[i].weight);
    }
  }

  void qdft(const voyx::vector<T> samples, voyx::matrix<std::complex<F>> dfts)
  {
    for (size_t i = 0; i < samples.size(); ++i)
    {
      qdft(samples[i], dfts[i]);
    }
  }

  T iqdft(const voyx::vector<std::complex<F>> dft)
  {
    F sample = F(0);

    for (size_t i = 0; i < dft.size(); ++i)
    {
      sample += (dft[i] * data[i].twiddle).real();
    }

    return static_cast<T>(sample);
  }

  void iqdft(const voyx::matrix<std::complex<F>> dfts, voyx::vector<T> samples)
  {
    for (size_t i = 0; i < samples.size(); ++i)
    {
      samples[i] = iqdft(dfts[i]);
    }
  }

private:

  struct qdft_config_t
  {
    double samplerate;
    std::pair<double, double> roi;
    double resolution;
    double latency;
    double quality;
    size_t size;
    std::vector<double> frequencies;
  };

  qdft_config_t config;

  struct qdft_data_t
  {
    size_t period;
    size_t offset;
    double weight;
    std::complex<F> twiddle;
  };

  std::vector<qdft_data_t> data;

  struct qdft_buffer_t
  {
    std::vector<T> input;
    std::vector<std::complex<F>> output;
  };

  qdft_buffer_t buffer;

};
