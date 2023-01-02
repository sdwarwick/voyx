#pragma once

#include <voyx/Header.h>
#include <voyx/io/Source.h>
#include <voyx/sign/Oscillator.h>

class SineSource : public Source<sample_t>
{

public:

  SineSource(double frequency, double samplerate, size_t framesize, size_t buffersize);
  SineSource(double amplitude, double frequency, double samplerate, size_t framesize, size_t buffersize);

  bool read(const size_t index, std::function<void(const voyx::vector<sample_t> frame)> callback) override;

private:

  const double amplitude;
  const double frequency;

  Oscillator<double> osc;

  std::vector<sample_t> frame;

};
