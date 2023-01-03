#include <voyx/dsp/QdftTestPipeline.h>

#include <voyx/Source.h>

QdftTestPipeline::QdftTestPipeline(const double samplerate, const size_t framesize,
                                   std::shared_ptr<Source<sample_t>> source, std::shared_ptr<Sink<sample_t>> sink,
                                   std::shared_ptr<MidiObserver> midi, std::shared_ptr<Plot> plot) :
  QdftPipeline(samplerate, framesize, source, sink),
  midi(midi),
  plot(plot)
{
  if (plot != nullptr)
  {
    const auto freqs = frequencies();

    plot->xmap([freqs](size_t i) { return freqs[i]; });
    plot->xlim(0, 2e3);
    plot->ylim(-120, 0);
  }
}

void QdftTestPipeline::operator()(const size_t index,
                                  voyx::matrix<phasor_t> dfts)
{
  if (plot != nullptr)
  {
    const auto dft = dfts.front();

    std::vector<double> abs(dft.size());

    for (size_t i = 0; i < dft.size(); ++i)
    {
      abs[i] = std::abs(dft[i]);
    }

    for (size_t i = 0; i < dft.size(); ++i)
    {
      abs[i] = 20 * std::log10(abs[i]);
    }

    plot->plot(abs);
  }
}
