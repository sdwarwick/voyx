#pragma once

#include <voyx/Header.h>
#include <voyx/dsp/SyncPipeline.h>

class BypassPipeline : public SyncPipeline<sample_t>
{

public:

  BypassPipeline(std::shared_ptr<Source<sample_t>> source, std::shared_ptr<Sink<sample_t>> sink);

protected:

  void operator()(const size_t index, const voyx::vector<sample_t> input, voyx::vector<sample_t> output) override;

};
