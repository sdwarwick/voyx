#pragma once

#include <voyx/Header.h>

/**
 * Modulated Sliding DFT implementation according to [1] combined with [2].
 *
 * [1] Krzysztof Duda
 *     Accurate, Guaranteed Stable, Sliding Discrete Fourier Transform
 *     IEEE Signal Processing Magazine (2010)
 *     https://ieeexplore.ieee.org/document/5563098
 *
 * [2] Russell Bradford and Richard Dobson and John ffitch
 *     Sliding is Smoother than Jumping
 *     International Computer Music Conference (2005)
 *     https://quod.lib.umich.edu/cgi/p/pod/dod-idx/sliding-is-smoother-than-jumping.pdf?c=icmc;idno=bbp2372.2005.086;format=pdf
 **/
template <typename T, typename F>
class SDFT
{

public:

  SDFT(const size_t dftsize, const double latency = 1) :
    dftsize(dftsize),
    latency(latency)
  {
    analysis.roi = { 0, dftsize };
    synthesis.roi = { 0, dftsize };

    analysis.twiddles.resize(dftsize);
    synthesis.twiddles.resize(dftsize);

    analysis.cursor = 0;
    analysis.maxcursor = dftsize * 2 - 1;
    analysis.input.resize(dftsize * 2);

    analysis.accoutput.resize(dftsize);
    analysis.auxoutput.resize(dftsize + 2);
    analysis.fiddles.resize(dftsize, 1);

    const F pi = F(-2) * std::acos(F(-1)) / (dftsize * 2);
    const F weight = F(2) / (F(1) - std::cos(pi * dftsize * F(latency)));

    for (size_t i = 0; i < dftsize; ++i)
    {
      analysis.twiddles[i] = std::polar(F(1), pi * i);
      synthesis.twiddles[i] = std::polar(weight, pi * i * dftsize * F(latency));
    }
  }

  size_t size() const
  {
    return dftsize;
  }

  void sdft(const T sample, voyx::vector<std::complex<F>> dft)
  {
    voyxassert(dft.size() == dftsize);

    // NOTE
    // actually the weight denominator needs to be dftsize*2 to get proper magnitude scaling,
    // but then requires a multiplication by factor 2 in synthesis and is therefore omitted

    const F weight = F(1) / dftsize;

    const F delta = sample - std::exchange(analysis.input[analysis.cursor], sample);

    for (size_t i = analysis.roi.first, j = i + 1; i < analysis.roi.second; ++i, ++j)
    {
      analysis.accoutput[i] += delta * analysis.fiddles[i];
      analysis.fiddles[i] *= analysis.twiddles[i];
      analysis.auxoutput[j] = analysis.accoutput[i] * std::conj(analysis.fiddles[i]);
    }

    analysis.auxoutput[0] = std::conj(analysis.auxoutput[2]);
    analysis.auxoutput[dftsize + 1] = std::conj(analysis.auxoutput[dftsize - 1]);

    for (size_t i = analysis.roi.first, j = i + 1; i < analysis.roi.second; ++i, ++j)
    {
      dft[i] = window(analysis.auxoutput[j - 1],
                      analysis.auxoutput[j],
                      analysis.auxoutput[j + 1],
                      weight);
    }

    if (++analysis.cursor > analysis.maxcursor)
    {
      analysis.cursor = 0;

      // TODO reset fiddles?
      // std::fill(analysis.fiddles.begin(), analysis.fiddles.end(), 1);
    }
  }

  void sdft(const voyx::vector<T> samples, voyx::matrix<std::complex<F>> dfts)
  {
    voyxassert(samples.size() == dfts.size());

    for (size_t i = 0; i < samples.size(); ++i)
    {
      sdft(samples[i], dfts[i]);
    }
  }

  T isdft(const voyx::vector<std::complex<F>> dft)
  {
    voyxassert(dft.size() == dftsize);

    F sample = F(0);

    if (latency == 1)
    {
      for (size_t i = synthesis.roi.first; i < synthesis.roi.second; ++i)
      {
        sample += dft[i].real() * (i % 2 ? -1 : +1);
      }
    }
    else
    {
      for (size_t i = synthesis.roi.first; i < synthesis.roi.second; ++i)
      {
        sample += (dft[i] * synthesis.twiddles[i]).real();
      }
    }

    return static_cast<T>(sample);
  }

  void isdft(const voyx::matrix<std::complex<F>> dfts, voyx::vector<T> samples)
  {
    voyxassert(samples.size() == dfts.size());

    for (size_t i = 0; i < samples.size(); ++i)
    {
      samples[i] = isdft(dfts[i]);
    }
  }

private:

  const size_t dftsize;
  const double latency;

  struct
  {
    std::pair<size_t, size_t> roi;
    std::vector<std::complex<F>> twiddles;

    size_t cursor;
    size_t maxcursor;
    std::vector<T> input;
    std::vector<std::complex<F>> accoutput;
    std::vector<std::complex<F>> auxoutput;
    std::vector<std::complex<F>> fiddles;
  }
  analysis;

  struct
  {
    std::pair<size_t, size_t> roi;
    std::vector<std::complex<F>> twiddles;
  }
  synthesis;

  inline static std::complex<F> window(const std::complex<F>& left,
                                       const std::complex<F>& middle,
                                       const std::complex<F>& right,
                                       const F weight)
  {
    return F(0.25) * ((middle + middle) - (left + right)) * weight;
  }

};
