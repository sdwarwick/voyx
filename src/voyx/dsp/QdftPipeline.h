#pragma once

#include <voyx/Header.h>
#include <voyx/alg/QDFT.h>
#include <voyx/dsp/SyncPipeline.h>

template<typename T = sample_t>
class QdftPipeline : public SyncPipeline<sample_t>
{

public:

  QdftPipeline(const double samplerate, const size_t framesize, std::shared_ptr<Source<sample_t>> source, std::shared_ptr<Sink<sample_t>> sink) :
    SyncPipeline<sample_t>(source, sink),
    samplerate(samplerate),
    framesize(framesize),
    qdft(samplerate, { 50, 20000 }, 24, 1)
  {
    data.dfts.resize(framesize * qdft.size());
  }

protected:

  const double samplerate;
  const size_t framesize;

  const std::vector<double>& frequencies() const
  {
    return qdft.frequencies();
  }

  void operator()(const size_t index, const voyx::vector<sample_t> input, voyx::vector<sample_t> output) override
  {
    voyx::matrix<phasor_t> dfts(data.dfts, qdft.size());

    qdft.qdft(input, dfts);
    (*this)(index, dfts);
    qdft.iqdft(dfts, output);
  }

  virtual void operator()(const size_t index, voyx::matrix<phasor_t> dfts) = 0;

private:

  QDFT<sample_t, phasor_t::value_type> qdft;

  struct
  {
    std::vector<phasor_t> dfts;
  }
  data;

};
