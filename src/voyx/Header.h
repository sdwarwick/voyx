#pragma once

/**
 * This file provides a set of common std modules
 * and is to be included in .h project files.
 **/

#include <algorithm>
#include <array>
#include <bitset>
#include <cctype>
#include <chrono>
#include <cmath>
#include <complex>
#include <condition_variable>
#include <csignal>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <numeric>
#include <optional>
#include <random>
#include <regex>
#include <semaphore>
#include <set>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <vector>

#include <numbers>

/**
 * Following by custom types.
 **/

#include <xtensor/xadapt.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xindex_view.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xsort.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>

#include <voyx/etc/Assert.h>
#include <voyx/etc/Vector.h>
#include <voyx/etc/Matrix.h>

/**
 * And finally common data type definitions.
 **/

typedef float sample_t;                // time domain
typedef std::complex<double> phasor_t; // frequency domain
