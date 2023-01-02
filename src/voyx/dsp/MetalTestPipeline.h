#ifdef VOYXMETAL

#pragma once

#include <voyx/Header.h>
#include <voyx/dsp/AsyncPipeline.h>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

class MetalTestPipeline : public AsyncPipeline<sample_t>
{

public:

  MetalTestPipeline(const double samplerate, const size_t framesize, const size_t dftsize, std::shared_ptr<Source<sample_t>> source, std::shared_ptr<Sink<sample_t>> sink);

protected:

  const double samplerate;
  const size_t framesize;
  const size_t dftsize;

  void begin(const size_t index, const voyx::vector<sample_t> input) override;

private:

};

#endif
