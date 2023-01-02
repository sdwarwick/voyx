#include <voyx/io/SweepSource.h>

#include <voyx/Source.h>

SweepSource::SweepSource(std::pair<double, double> frequencies, double period, double samplerate, size_t framesize, size_t buffersize) :
  SweepSource(1, frequencies, period, samplerate, framesize, buffersize)
{
}

SweepSource::SweepSource(double amplitude, std::pair<double, double> frequencies, double period, double samplerate, size_t framesize, size_t buffersize) :
  Source(samplerate, framesize, buffersize),
  amplitude(amplitude),
  frequencies(frequencies),
  period(period),
  osc(frequencies, period, samplerate),
  frame(framesize)
{
}

bool SweepSource::read(const size_t index, std::function<void(const voyx::vector<sample_t> frame)> callback)
{
  for (size_t i = 0; i < frame.size(); ++i)
  {
    frame[i] = amplitude * osc.sin();
  }

  callback(frame);

  return true;
}
