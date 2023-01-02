#pragma once

#include <voyx/Header.h>

#include <RtMidi.h>

class MidiObserver
{

public:

  MidiObserver(const std::string& name, const double concertpitch);
  ~MidiObserver();

  double concertpitch() const;

  std::vector<int> state();
  std::vector<double> frequencies();

  std::vector<double> mask();
  std::vector<double> imask();

  bool sustain();

  void start();
  void stop();

private:

  const std::string midi_device_name;
  const double midi_concert_pitch;

  std::vector<int> midi_key_state;
  bool midi_control_sustain;

  RtMidiIn midi;

  std::mutex mutex;

  static void dump(std::vector<unsigned char>* message);
  static void callback(double timestamp, std::vector<unsigned char>* message, void* $this);
  static void error(RtMidiError::Type type, const std::string& error, void* $this);

};
