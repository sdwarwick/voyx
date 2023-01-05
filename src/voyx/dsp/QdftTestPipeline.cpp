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
    plot->xlog();
    plot->xlim(50, 5e3);
    plot->ylim(-120, 0);
  }
}

void QdftTestPipeline::operator()(const size_t index,
                                  voyx::matrix<phasor_t> dfts)
{
  if (plot != nullptr)
  {
    const auto dft = dfts.front().ndarray();

    auto abs = nc::abs(dft);

    const auto freqs = $$::ndarray(frequencies());
    const auto peaks = $$::findpeaks(abs, 3);

    const auto abspeak = nc::argmax(abs);
    const auto freqpeak = freqs[abspeak].front();

    abs = 20.0 * nc::log10(abs);

    const auto xpeaks = freqs[peaks];
    const auto ypeaks = abs[peaks];

    plot->plot(abs);
    plot->scatter(xpeaks, ypeaks);
    plot->xline(freqpeak);
  }
}
