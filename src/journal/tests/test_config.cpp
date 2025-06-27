#include "doctest.h"
#include "fakeit.hpp"
#include "..\underworld.h"

using namespace fakeit;

TEST_CASE("Invalid") {
    IConfig* config = create_config("test");

    char* argv[] = { (char*)"program", (char*)"-invalid" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = config->parse(argc, argv);

    CHECK_FALSE(result);
}

TEST_CASE("Blog") {
    IConfig* config = create_config("test");

    char* argv[] = { (char*)"program", (char*)"blog" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = config->parse(argc, argv);

    CHECK(result);
    CHECK(config->is_blog());
    CHECK_FALSE(config->is_book());
}

TEST_CASE("Book") {
    IConfig* config = create_config("test");

    char* argv[] = { (char*)"program", (char*)"book" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = config->parse(argc, argv);

    CHECK(result);
    CHECK(config->is_book());
    CHECK_FALSE(config->is_blog());
}

TEST_CASE("Both") {
    IConfig* config = create_config("test");

    char* argv[] = { (char*)"program", (char*)"both" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = config->parse(argc, argv);

    CHECK(result);
    CHECK(config->is_book());
    CHECK(config->is_blog());
}

