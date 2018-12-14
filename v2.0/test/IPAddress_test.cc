//
// Created by Crow on 12/6/18.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "net/socket.h"
#include "net/ip_address.h"

TEST_CASE("IPAddress", "[single-file]") {

  platinum::IPAddress addr(9877);

  SECTION("test 1") {
    CHECK(addr.port() == 9877);
  }

  SECTION("test 2") {
    addr.set_port(1234);
    CHECK(addr.port() == 1234);
    REQUIRE(addr.port() == 1234);
  }

  SECTION("test 3") {
    REQUIRE(addr.ip() == "0.0.0.0");
  }

  SECTION("test 4") {
    addr.set_ip("12334.56.78.9");
    REQUIRE(addr.ip() == "12334.56.78.9");
  }
}
