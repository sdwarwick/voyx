#pragma once

#include <voyx/Header.h>
#include <voyx/io/Source.h>
#include <voyx/sign/Noise.h>

class NoiseSource : public Source<sample_t>
{

public:

  NoiseSource(double samplerate, size_t framesize, size_t buffersize);
  NoiseSource(double amplitude, double samplerate, size_t framesize, size_t buffersize);

  bool read(const size_t index, std::function<void(const voyx::vector<sample_t> frame)> callback) override;

private:

  const double amplitude;

  Noise<double> noise;

  std::vector<sample_t> frame;

};
