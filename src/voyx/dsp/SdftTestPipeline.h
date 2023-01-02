#pragma once

#include <voyx/Header.h>
#include <voyx/alg/Vocoder.h>
#include <voyx/dsp/SdftPipeline.h>
#include <voyx/io/MidiObserver.h>
#include <voyx/ui/Plot.h>

class SdftTestPipeline : public SdftPipeline<double>
{

public:

  SdftTestPipeline(const double samplerate, const size_t framesize, const size_t dftsize,
                   std::shared_ptr<Source<sample_t>> source, std::shared_ptr<Sink<sample_t>> sink,
                   std::shared_ptr<MidiObserver> midi, std::shared_ptr<Plot> plot);

  void operator()(const size_t index,
                  voyx::matrix<phasor_t> dfts) override;

private:

  Vocoder<double> vocoder;

  std::shared_ptr<MidiObserver> midi;
  std::shared_ptr<Plot> plot;

};
