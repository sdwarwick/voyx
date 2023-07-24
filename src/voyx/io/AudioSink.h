#pragma once

#include <voyx/Header.h>
#include <voyx/alg/SRC.h>
#include <voyx/etc/FIFO.h>
#include <voyx/io/Sink.h>

#include <RtAudio.h>

class AudioSink : public Sink<sample_t>
{

public:

  AudioSink(const std::string& name, double samplerate, size_t framesize, size_t buffersize);

  void open() override;
  void close() override;

  void start() override;
  void stop() override;

  bool write(const size_t index, const voyx::vector<sample_t> frame) override;
  bool sync() override;

private:

  struct OutputFrame
  {
    size_t index;
    std::vector<sample_t> frame;
  };

  const std::string audio_device_name;
  std::counting_semaphore<> audio_sync_semaphore;
  FIFO<OutputFrame> audio_frame_buffer;
  SRC<sample_t> audio_samplerate_converter;

  RtAudio audio;

  static int callback(void* output_frame_data, void* input_frame_data, uint32_t framesize, double timestamp, RtAudioStreamStatus status, void* $this);
  static void error(RtAudioError::Type type, const std::string& error);

};
