#ifndef CH9325_HPP_
#define CH9325_HPP_ 1

#include "hidapi/hidapi.h"

#include <array>
#include <cstdint>
#include <string>

// allows reading data from CH9325 serial communications ICs
class Ch9325 {
  hid_device *handle_;
  int bps_, dlen_;

  Ch9325(hid_device *h, int bit_rate_bps, int data_length_bits);

  void apply_settings_();

 public:
  explicit Ch9325(const char *path, int bit_rate_bps = 2400,
                  int data_length_bits = 8);
  Ch9325(uint16_t vendor, uint16_t device, int bit_rate_bps = 2400,
         int data_length_bits = 8);

  std::pair<int, std::array<char, 7>> read();
};

#endif // include guard
