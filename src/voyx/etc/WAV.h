#pragma once

#include <voyx/Header.h>

struct WAV
{
  static void read(const std::string& path, std::vector<double>& data, const double samplerate);
  static void read(const std::string& path, std::vector<float>& data, const double samplerate);

  static void write(const std::string& path, const std::vector<double>& data, const double samplerate);
  static void write(const std::string& path, const std::vector<float>& data, const double samplerate);
};
