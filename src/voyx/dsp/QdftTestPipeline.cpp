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
    const auto dft = dfts.front().xarray();

    auto&& abs = xt::eval(xt::abs(dft));
    auto&& absdb = xt::eval($$::db(abs));

    const auto freqs = xt::adapt(frequencies());
    const auto peaks = $$::findpeaks(abs, 3);

    const auto ipeak = xt::argmax(abs)();
    const auto fpeak = freqs[ipeak];

    const auto xpeaks = xt::index_view(freqs, peaks);
    const auto ypeaks = xt::index_view(absdb, peaks);

    const auto weights = $$::a_weighting(freqs);
    const auto loudness = $$::db(xt::amax(abs * weights))();

    plot->plot(absdb);
    plot->scatter(xt::eval(xpeaks), xt::eval(ypeaks));
    plot->xline(fpeak);
    plot->yline(loudness);
  }
}
