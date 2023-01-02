#include <voyx/dsp/RobotPipeline.h>

#include <voyx/Source.h>

RobotPipeline::RobotPipeline(const double samplerate, const size_t framesize, const size_t dftsize,
                             std::shared_ptr<Source<sample_t>> source, std::shared_ptr<Sink<sample_t>> sink,
                             std::shared_ptr<MidiObserver> midi, std::shared_ptr<Plot> plot) :
  SdftPipeline(samplerate, framesize, dftsize, source, sink),
  midi(midi),
  plot(plot)
{
}

void RobotPipeline::operator()(const size_t index,
                               voyx::matrix<phasor_t> dfts)
{
  std::set<double> frequencies;
  bool sustain = false;

  if (midi != nullptr)
  {
    const auto values = midi->frequencies();
    frequencies.insert(values.begin(), values.end());

    sustain = midi->sustain();
  }

  if (sustain)
  {
    frequencies.merge(this->frequencies);
  }

  this->frequencies = frequencies;

  for (const double frequency : frequencies)
  {
    if (osc.count(frequency))
    {
      continue;
    }

    osc[frequency].resize(dftsize);

    for (size_t i = 0; i < dftsize; ++i)
    {
      osc[frequency][i] = { i * frequency, samplerate };
    }
  }

  std::vector<double> abs(dftsize);

  for (size_t i = 0; i < dfts.size(); ++i)
  {
    auto dft = dfts[i];

    for (size_t j = 0; j < dft.size(); ++j)
    {
      abs[j] = std::abs(dft[j]);
      dft[j] = 0;
    }

    for (const double frequency : frequencies)
    {
      for (size_t j = 0; j < dft.size(); ++j)
      {
        dft[j] += osc[frequency][j];
      }
    }

    const double weight = frequencies.empty()
      ? 0.0 : 1.0 / frequencies.size();

    for (size_t j = 0; j < dft.size(); ++j)
    {
      dft[j] *= abs[j] * weight;
    }
  }
}
