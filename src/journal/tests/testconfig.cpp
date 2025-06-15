#include "doctest.h"
#include "fakeit.hpp"
#include "..\config.h"

using namespace fakeit;

TEST_CASE("Invalid") {
    Config config;

    char* argv[] = { (char*)"program", (char*)"-invalid" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = config.parse(argc, argv);

    CHECK_FALSE(result);
}

TEST_CASE("Blog") {
    Config config;

    char* argv[] = { (char*)"program", (char*)"blog" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = config.parse(argc, argv);

    CHECK(result);
    CHECK(config.is_blog());
    CHECK_FALSE(config.is_book());
}

TEST_CASE("Book") {
    Config config;

    char* argv[] = { (char*)"program", (char*)"book" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = config.parse(argc, argv);

    CHECK(result);
    CHECK(config.is_book());
    CHECK_FALSE(config.is_blog());
}

TEST_CASE("Both") {
    Config config;

    char* argv[] = { (char*)"program", (char*)"both" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = config.parse(argc, argv);

    CHECK(result);
    CHECK(config.is_book());
    CHECK(config.is_blog());
}

