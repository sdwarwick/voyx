#ifdef VOYXOPENCL

#pragma once

#include <voyx/Header.h>
#include <voyx/dsp/SyncPipeline.h>

#include <CL/opencl.hpp>

class OpenclTestPipeline : public SyncPipeline<sample_t>
{

public:

  OpenclTestPipeline(const double samplerate, const size_t framesize, const size_t dftsize, std::shared_ptr<Source<sample_t>> source, std::shared_ptr<Sink<sample_t>> sink);

protected:

  const double samplerate;
  const size_t framesize;
  const size_t dftsize;

  void operator()(const size_t index, const voyx::vector<sample_t> input, voyx::vector<sample_t> output) override;

private:

  cl::Device device;
  cl::Context context;
  cl::CommandQueue queue;
  cl::Program program;

  cl::Kernel sdft1;
  cl::Kernel sdft2;

  struct
  {
    std::vector<sample_t> inputs;
    std::vector<phasor_t> outputs;
    std::vector<phasor_t> dfts;
    size_t cursor;
  }
  buffer;

const std::string kernels =
  R"(

    typedef float sample_t;

    kernel void sdft1(global const sample_t* samples,
                      global const sample_t* samplebuffer,
                      global sample_t* dftbuffer,
                      const int framesize,
                      const int dftsize,
                      const int cursor)
    {
      const int dftbin = get_global_id(0);

      const int dftstride = dftsize + 2;

      const sample_t pi = sample_t(2) * dftbin / dftsize;
      const sample_t twiddle[] = { cospi(pi), sinpi(pi) };

      for (int sample = 0; sample < framesize; ++sample)
      {
        const int i = (sample * dftstride + (dftbin + 1)) * 2;
        const int j = ((sample ? sample - 1 : framesize - 1) * dftstride + (dftbin + 1)) * 2;

        dftbuffer[i + 0] = dftbuffer[j + 0];
        dftbuffer[i + 1] = dftbuffer[j + 1];

        const int oldsample = (cursor + sample) % dftsize;

        const sample_t delta = samples[sample] - samplebuffer[oldsample];

        dftbuffer[i + 0] += delta;
        dftbuffer[i + 1] += delta;

        const sample_t real = dftbuffer[i + 0] * twiddle[0] - dftbuffer[i + 1] * twiddle[1];
        const sample_t imag = dftbuffer[i + 0] * twiddle[1] + dftbuffer[i + 1] * twiddle[0];

        dftbuffer[i + 0] = real;
        dftbuffer[i + 1] = imag;

        // skip dft buffer circularity fix to save gpu cycles

        // const int dftoffset = sample * dftstride * 2;

        // for (int j = 0; j < 2; ++j)
        // {
        //   dftbuffer[dftoffset + j] = dftbuffer[dftoffset + dftsize * 2 + j];
        //   dftbuffer[dftoffset + (dftsize + 1) * 2 + j] = dftbuffer[dftoffset + 2 + j];
        // }
      }
    }

    kernel void sdft2(global const sample_t* dftbuffer,
                      global sample_t* dfts,
                      const int dftsize,
                      const sample_t scale)
    {
      const int dftbin = get_global_id(0);
      const int sample = get_global_id(1);

      const int dftstride = dftsize + 2;

      const int i = (sample * dftsize + dftbin) * 2;
      const int l = (sample * dftstride + dftbin + 0) * 2;
      const int m = (sample * dftstride + dftbin + 1) * 2;
      const int r = (sample * dftstride + dftbin + 2) * 2;

      for (int j = 0; j < 2; ++j)
      {
        const sample_t left = dftbuffer[l + j];
        const sample_t middle = dftbuffer[m + j];
        const sample_t right = dftbuffer[r + j];

        dfts[i + j] = sample_t(0.25) * ((middle + middle) - (left + right)) * scale;
      }
    }

  )";

};

#endif
