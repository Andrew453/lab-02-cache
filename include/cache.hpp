// Copyright 2021 Andrew Prokushev <senior.prockuschev2017@yandex.ru>

#ifndef INCLUDE_CACHE_HPP_
#define INCLUDE_CACHE_HPP_

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

struct Statistics {
  int experimentNumber_front;
  int experimentNumber_reverse;
  int experimentNumber_random;
  int bufferSize;
  double duration_front;
  double duration_reverse;
  double duration_random;
};

class CacheDiagnostics {
 private:
  unsigned int seed;
  int read_value;
  int sizes[5];
  int* arr;
  void create_array(int num_size);

 public:
  Statistics statistics[5];
  CacheDiagnostics();
  void front_diagnostics(int num_size);
  void reverse_diagnostics(int num_size);
  void random_diagnostics(int num_size);
  void formatting_output();
  void full_diagnostics();
    ~CacheDiagnostics();
};

/*  Intel Core i5 8265U
 * cache_sizes['1'] = 256 KB
 * cache_sizes['2'] = 1024 KB
 * cache_sizes['3'] = 6144 KB
 * cache_sizes['1']*1/2 = 128 KB < 256 KB < 1024 KB < 6144 KB < 9216 KB =
 * cache_sizes['3']*3/2
 */

#endif  // INCLUDE_CACHE_HPP_
