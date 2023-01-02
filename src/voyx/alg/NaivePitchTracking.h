#pragma once

#include <voyx/Header.h>
#include <voyx/etc/Convert.h>

class NaivePitchTracking
{

public:

  NaivePitchTracking(const double concertpitch) :
    concertpitch(concertpitch)
  {
  }

  double operator()(const double f0) const
  {
    const double key = std::round($$::midi::key(f0, concertpitch));
    const double freq = $$::midi::freq(key, concertpitch);

    return freq;
  }

private:

  const double concertpitch;

};
