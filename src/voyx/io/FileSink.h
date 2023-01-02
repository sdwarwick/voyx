#pragma once

#include <voyx/Header.h>
#include <voyx/io/Sink.h>

class FileSink : public Sink<sample_t>
{

public:

  FileSink(const std::string& path, double samplerate, size_t framesize, size_t buffersize);

  void open() override;
  void close() override;

  bool write(const size_t index, const voyx::vector<sample_t> frame) override;

private:

  const std::string path;

  std::vector<sample_t> data;

};
