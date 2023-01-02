#pragma once

#include <voyx/Header.h>
#include <voyx/io/Source.h>

class FileSource : public Source<sample_t>
{

public:

  FileSource(const std::string& path, double samplerate, size_t framesize, size_t buffersize);

  void open() override;
  void close() override;

  bool read(const size_t index, std::function<void(const voyx::vector<sample_t> frame)> callback) override;

private:

  const std::string path;

  std::vector<sample_t> data;
  std::vector<sample_t> frame;

};
