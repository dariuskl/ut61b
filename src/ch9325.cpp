#include "ch9325.hpp"

#include <array>

namespace {
  const char *hid_error_(hid_device *const h, const char *const fallback) {
    static auto buf = std::array<char, 256>{};
    const auto *const error_string = hid_error(h);
    if (error_string != nullptr) {
      wcstombs(buf.data(), error_string, buf.size());
    } else {
      snprintf(buf.data(), buf.size(), "%s", fallback);
    }
    return buf.data();
  }
} // namespace

Ch9325::Ch9325(hid_device *h, const int bit_rate_bps,
               const int data_length_bits)
    : handle_{h}, bps_{bit_rate_bps}, dlen_{data_length_bits} {
  if (handle_ == nullptr) {
    throw std::runtime_error{"could not open device"};
  }
  // TODO: set power to 'auto', autosuspend to '0'
  apply_settings_();
}

Ch9325::Ch9325(const char *const path, const int bit_rate_bps,
               const int data_length_bits)
    : Ch9325{hid_open_path(path), bit_rate_bps, data_length_bits} {}

Ch9325::Ch9325(const uint16_t vendor, const uint16_t device, int bit_rate_bps,
               int data_length_bits)
    : Ch9325{hid_open(vendor, device, nullptr), bit_rate_bps,
             data_length_bits} {}

void Ch9325::apply_settings_() {
  const auto message = std::array<std::uint8_t, 6>{{
      0U, // endpoint 0
      static_cast<uint8_t>(bps_), static_cast<uint8_t>(bps_ >> 8U),
      0U, //  uncertain purpose, probably parity and ...
      0U, // ...stop bits specs, often omitted, or 0x00 is sent
      static_cast<uint8_t>(dlen_) // number of data bits (encoded, values 0-3
                                  // translate to 5-8 data bits)
  }};
  if (hid_send_feature_report(handle_, message.data(), message.size()) < 0) {
    throw std::runtime_error{
        hid_error_(handle_, "could not send feature report")};
  }
}

std::pair<int, std::array<char, 7>> Ch9325::read() {
  if (auto buf = std::array<uint8_t, 8>{0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U};
      hid_read(handle_, buf.data(), buf.size()) < 0) {
    throw std::runtime_error{hid_error_(handle_, "could not read from device")};
  } else {
    const auto length = static_cast<int>(buf[0] & 0xfU);
    return std::make_pair(
        length, std::array<char, 7>{{static_cast<char>(buf[1] & 0x7fU),
                                     static_cast<char>(buf[2] & 0x7fU),
                                     static_cast<char>(buf[3] & 0x7fU),
                                     static_cast<char>(buf[4] & 0x7fU),
                                     static_cast<char>(buf[5] & 0x7fU),
                                     static_cast<char>(buf[6] & 0x7fU),
                                     static_cast<char>(buf[7] & 0x7fU)}});
  }
}
