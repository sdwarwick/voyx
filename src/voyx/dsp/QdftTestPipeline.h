#pragma once

#include <voyx/Header.h>
#include <voyx/dsp/QdftPipeline.h>
#include <voyx/io/MidiObserver.h>
#include <voyx/ui/Plot.h>

class QdftTestPipeline : public QdftPipeline<double>
{

public:

  QdftTestPipeline(const double samplerate, const size_t framesize,
                   std::shared_ptr<Source<sample_t>> source, std::shared_ptr<Sink<sample_t>> sink,
                   std::shared_ptr<MidiObserver> midi, std::shared_ptr<Plot> plot);

  void operator()(const size_t index,
                  voyx::matrix<phasor_t> dfts) override;

private:

  std::shared_ptr<MidiObserver> midi;
  std::shared_ptr<Plot> plot;

};
