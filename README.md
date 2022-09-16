# voyx

The project idea is essentially to harmonize the source vocal audio signal just in real time depending on the MIDI input. Each MIDI key determines the particular pitch shifting amount relative to the detected fundamental frequency of the source signal segment.

The required multi pitch shifting feature is already implemented in the [stftPitchShift](https://github.com/jurihock/stftPitchShift) project. An additional idea is to utilize the [Sliding DFT](https://github.com/jurihock/sdft) instead of STFT.

Let's get started...
