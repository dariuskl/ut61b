#include "fs9922_dmm3.hpp"

#include <catch2/catch.hpp>

TEST_CASE("decoding", "[ut61b]") {
  auto uut = Fs9922_dmm3{};

  WHEN("pushing valid data") {
    const auto vectors = GENERATE(
        std::make_pair("+1234 0\x00\x00\x00\x00\x00\r\n", "+1234"),
        std::make_pair("+1234 1\x00\x00\x00\x00\x00\r\n", "+1.234"),
        std::make_pair("+1234 2\x00\x00\x00\x00\x00\r\n", "+12.34"),
        std::make_pair("+1234 4\x00\x00\x00\x00\x00\r\n", "+123.4"),
        std::make_pair("-1234 0\x00\x00\x00\x00\x00\r\n", "-1234"),
        std::make_pair("-1234 1\x00\x00\x00\x00\x00\r\n", "-1.234"),
        std::make_pair("-1234 2\x00\x00\x00\x00\x00\r\n", "-12.34"),
        std::make_pair("-1234 4\x00\x00\x00\x00\x00\r\n", "-123.4"),
        std::make_pair("-1234 4\x00\x00\x00\x01\x00\r\n", "-123.4 °F"),
        std::make_pair("-1234 4\x00\x00\x00\x02\x00\r\n", "-123.4 °C"),
        std::make_pair("-1234 4\x00\x00\x00\x04\x00\r\n", "-123.4 F"),
        std::make_pair("-1234 4\x00\x00\x00\x08\x00\r\n", "-123.4 Hz"),
        std::make_pair("-1234 4\x00\x00\x00\x10\x00\r\n", "-123.4 hFE"),
        std::make_pair("-1234 4\x00\x00\x00\x20\x00\r\n", "-123.4 Ω"),
        std::make_pair("-1234 4\x00\x00\x00\x40\x00\r\n", "-123.4 A"),
        std::make_pair("-1234 4\x00\x00\x00\x80\x00\r\n", "-123.4 V"),
        std::make_pair("-1234 4\x00\x00\x10\x80\x00\r\n", "-123.4 MV"),
        std::make_pair("-1234 4\x00\x00\x20\x80\x00\r\n", "-123.4 kV"),
        std::make_pair("-1234 4\x00\x00\x40\x80\x00\r\n", "-123.4 mV"),
        std::make_pair("-1234 4\x00\x00\x80\x80\x00\r\n", "-123.4 µV"));

    uut.push(vectors.first, 14);
    THEN("correctly decoded") { REQUIRE(uut.text() == vectors.second); }
  }
}
