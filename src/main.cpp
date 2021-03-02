#include "ch9325.hpp"
#include "fs9922_dmm3.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>

int main([[maybe_unused]] const int argc,
         [[maybe_unused]] const char *const *const argv) try {
  auto cable = Ch9325{0x1a86, 0xe008};
  auto decoder = Fs9922_dmm3{};
  auto log_file = std::ofstream{"./debug.log", std::ostream::out};
  while (true) {
    const auto data = cable.read();
    for (auto i = 0; i < data.first; ++i) {
      log_file << " " << std::hex << std::setw(2) << std::setfill('0')
               << int{data.second.at(static_cast<size_t>(i))};
      log_file.flush();
    }
    decoder.push(data.second.data(), data.first);
    if (const auto text = decoder.get_text(); text) {
      log_file << std::endl;
      std::cout << '\r' << std::setfill(' ') << std::setw(4)
                << (decoder.is_autorange() ? "AUTO" : "MANU") << " "
                << std::setw(2)
                << (decoder.is_dc() ? "DC" : (decoder.is_ac() ? "AC" : ""))
                << " " << std::setw(3) << (decoder.is_relative() ? "REL" : "")
                << "" << *text << " " << std::setw(4)
                << (decoder.is_hold() ? "HOLD" : "");
      std::flush(std::cout);
    }
  }
} catch (const std::exception &ex) {
  std::cerr << "fatal: " << ex.what() << std::endl;
}
