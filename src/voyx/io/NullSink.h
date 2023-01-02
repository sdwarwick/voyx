#pragma once

#include <voyx/Header.h>
#include <voyx/io/Sink.h>

class NullSink : public Sink<sample_t>
{

public:

  NullSink(double samplerate, size_t framesize, size_t buffersize);

  bool write(const size_t index, const voyx::vector<sample_t> frame) override;

};
