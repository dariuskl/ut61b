#ifndef FS9922_DMM3_HPP_
#define FS9922_DMM3_HPP_ 1

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>

class Fs9922_dmm3 {
  int state_{0};
  std::array<char, 8> buffer_;
  bool new_{false};
  std::string text_;
  double value_;
  std::array<uint8_t, 4> status_;
  int bar_;

 public:
  // measurement text with sign and unit and additional information where
  // applicable
  [[nodiscard]] std::string text() const { return text_; }

  // measured value as a floating point number
  [[nodiscard]] double value() const { return value_; }

  // whether or not the bargraph shall be shown
  [[nodiscard]] bool show_bargraph() const {
    return (status_[0] & 0x01U) != 0U;
  }

  // bargraph value
  [[nodiscard]] int bar() const { return bar_; }

  // the unit of the measured variable including its magnitude (e.g. mV)
  [[nodiscard]] std::string unit() const;

  [[nodiscard]] bool is_autorange() const { return (status_[0] & 0x20U) != 0U; }
  [[nodiscard]] bool is_dc() const { return (status_[0] & 0x10U) != 0U; }
  [[nodiscard]] bool is_ac() const { return (status_[0] & 0x08U) != 0U; }
  [[nodiscard]] bool is_relative() const { return (status_[0] & 0x04U) != 0U; }
  [[nodiscard]] bool is_hold() const { return (status_[0] & 0x02U) != 0U; }
  [[nodiscard]] bool z() const {
    return ((status_[1] & 0xc0U) >> 6U) | ((status_[1] & 1U) << 2U)
           | ((status_[2] & 1U) << 3U);
  }
  [[nodiscard]] bool n() const { return (status_[1] & 0x02U) != 0U; }
  [[nodiscard]] bool is_battery_low() const {
    return (status_[1] & 0x04U) != 0U;
  }
  [[nodiscard]] bool is_auto_power_off_active() const {
    return (status_[1] & 0x08U) != 0U;
  }
  [[nodiscard]] bool is_min() const { return (status_[1] & 0x10U) != 0U; }
  [[nodiscard]] bool is_max() const { return (status_[1] & 0x20U) != 0U; }
  [[nodiscard]] bool is_beep() const { return (status_[2] & 0x08U) != 0U; }
  [[nodiscard]] bool is_diode() const { return (status_[2] & 0x04U) != 0U; }
  [[nodiscard]] bool is_duty_cycle() const {
    return (status_[2] & 0x02U) != 0U;
  }

  std::optional<std::string> get_text() {
    if (new_) {
      new_ = false;
      return {text_};
    } else {
      return {};
    }
  }

  std::optional<double> get_value() {
    if (new_) {
      new_ = false;
      return {value_};
    } else {
      return {};
    }
  }

  void push(const char *cc, int num);
  void push(char c);
};

#endif // include guard
