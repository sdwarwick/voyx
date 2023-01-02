#include <voyx/dsp/BypassPipeline.h>

#include <voyx/Source.h>

BypassPipeline::BypassPipeline(std::shared_ptr<Source<sample_t>> source, std::shared_ptr<Sink<sample_t>> sink) :
  SyncPipeline<sample_t>(source, sink)
{
}

void BypassPipeline::operator()(const size_t index, const voyx::vector<sample_t> input, voyx::vector<sample_t> output)
{
  output = input;
}
