#pragma once

#include <voyx/Header.h>
#include <voyx/io/Source.h>

class NullSource : public Source<sample_t>
{

public:

  NullSource(double samplerate, size_t framesize, size_t buffersize);

  bool read(const size_t index, std::function<void(const voyx::vector<sample_t> frame)> callback) override;

private:

  const std::vector<sample_t> frame;

};
