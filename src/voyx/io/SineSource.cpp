#include <voyx/io/SineSource.h>

#include <voyx/Source.h>

SineSource::SineSource(double frequency, double samplerate, size_t framesize, size_t buffersize) :
  SineSource(1, frequency, samplerate, framesize, buffersize)
{
}

SineSource::SineSource(double amplitude, double frequency, double samplerate, size_t framesize, size_t buffersize) :
  Source(samplerate, framesize, buffersize),
  amplitude(amplitude),
  frequency(frequency),
  osc(frequency, samplerate),
  frame(framesize)
{
}

bool SineSource::read(const size_t index, std::function<void(const voyx::vector<sample_t> frame)> callback)
{
  for (size_t i = 0; i < frame.size(); ++i)
  {
    frame[i] = sample_t(amplitude * osc.sin());
  }

  callback(frame);

  return true;
}
