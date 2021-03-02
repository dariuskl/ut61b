#include "fs9922_dmm3.hpp"

#include <cassert>

namespace {

  constexpr auto scales_ = std::array<const char *, 5>{
      {"", "M", "k", "m", "µ"}};

  constexpr auto units_ = std::array<const char *, 9>{
      {"", "°F", "°C", "F", "Hz", "hFE", "Ω", "A", "V"}};

} // namespace

std::string Fs9922_dmm3::unit() const {
  return std::string{
             scales_.at(static_cast<size_t>(__builtin_ffs(status_[2] >> 4U)))}
         + std::string{
             units_.at(static_cast<size_t>(__builtin_ffs(status_[3])))};
}

void Fs9922_dmm3::push(const char *cc, const int num) {
  for (auto i = 0; i < num; ++i) {
    push(cc[i]);
  }
}

void Fs9922_dmm3::push(const char c) {
  switch (state_) {
  default:
    state_ = 0;
    [[fallthrough]];
  case 0:
    if ((c == '+') or (c == '-')) {
      buffer_[0] = c;
      ++state_;
    }
    break;

  case 1:
  case 2:
  case 3:
  case 4:
    buffer_.at(static_cast<size_t>(state_)) = c;
    ++state_;
    break;

  case 5:
    // space
    assert(c == ' ');
    ++state_;
    break;

  case 6: {
    auto s = '.';
    switch (c) {
    default:
    case '0':
      // no decimal point
      break;

    case '1':
      std::swap(buffer_[2], s);
      [[fallthrough]];

    case '2':
      std::swap(buffer_[3], s);
      [[fallthrough]];

    case '4': // datasheet says '3'; my meter says '4', which is in line with
              // https://www-user.tu-chemnitz.de/~heha/hsn/UNI-T/UT61BCD.LOG
      std::swap(buffer_[4], s);
      // finally:
      std::swap(buffer_[5], s);
      break;
    }
    ++state_;
    break;
  }

  case 7: // SB1
    status_[0] = static_cast<uint8_t>(c);
    ++state_;
    break;

  case 8: // SB2
    status_[1] = static_cast<uint8_t>(c);
    ++state_;
    break;

  case 9: // SB3
    status_[2] = static_cast<uint8_t>(c);
    ++state_;
    break;

  case 10: // SB4
    status_[3] = static_cast<uint8_t>(c);
    ++state_;
    break;

  case 11: // BAR
    bar_ = c;
    ++state_;
    break;

  case 12: // EOF
    ++state_;
    break;

  case 13: // ENTER
    new_ = true;
    const auto u = unit();
    if (not u.empty()) {
      text_ = std::string{buffer_.data()} + std::string{" "} + u;
    } else {
      text_ = std::string{buffer_.data()};
    }
    buffer_.fill('\0');
    state_ = 0;
    break;
  }
}
