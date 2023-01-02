#pragma once

#include <voyx/Header.h>
#include <voyx/alg/SDFT.h>
#include <voyx/dsp/SyncPipeline.h>

template<typename T = sample_t>
class SdftPipeline : public SyncPipeline<sample_t>
{

public:

  SdftPipeline(const double samplerate, const size_t framesize, const size_t dftsize, std::shared_ptr<Source<sample_t>> source, std::shared_ptr<Sink<sample_t>> sink) :
    SyncPipeline<sample_t>(source, sink),
    samplerate(samplerate),
    framesize(framesize),
    dftsize(dftsize),
    sdft(dftsize)
  {
    data.dfts.resize(framesize * dftsize);
  }

protected:

  const double samplerate;
  const size_t framesize;
  const size_t dftsize;

  void operator()(const size_t index, const voyx::vector<sample_t> input, voyx::vector<sample_t> output) override
  {
    voyx::matrix<phasor_t> dfts(data.dfts, dftsize);

    sdft.sdft(input, dfts);
    (*this)(index, dfts);
    sdft.isdft(dfts, output);
  }

  virtual void operator()(const size_t index, voyx::matrix<phasor_t> dfts) = 0;

private:

  SDFT<sample_t, phasor_t::value_type> sdft;

  struct
  {
    std::vector<phasor_t> dfts;
  }
  data;

};
