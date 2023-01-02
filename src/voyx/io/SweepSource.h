#pragma once

#include <voyx/Header.h>
#include <voyx/io/Source.h>
#include <voyx/sign/Wobbulator.h>

class SweepSource : public Source<sample_t>
{

public:

  SweepSource(std::pair<double, double> frequencies, double period, double samplerate, size_t framesize, size_t buffersize);
  SweepSource(double amplitude, std::pair<double, double> frequencies, double period, double samplerate, size_t framesize, size_t buffersize);

  bool read(const size_t index, std::function<void(const voyx::vector<sample_t> frame)> callback) override;

private:

  const double amplitude;
  const std::pair<double, double> frequencies;
  const double period;

  Wobbulator<double> osc;

  std::vector<sample_t> frame;

};
