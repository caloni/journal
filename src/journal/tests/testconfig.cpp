#include "doctest.h"
#include "fakeit.hpp"
#include "..\config.h"

using namespace fakeit;

TEST_CASE("Invalid") {
    Mock<IShell> shellMock;
    Config config(shellMock.get());

    char* argv[] = { (char*)"program", (char*)"-invalid" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = config.parse(argc, argv);

    CHECK_FALSE(result);
}

TEST_CASE("Blog") {
    Mock<IShell> shellMock;
    Config config(shellMock.get());

    char* argv[] = { (char*)"program", (char*)"blog" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = config.parse(argc, argv);

    CHECK(result);
    CHECK(config.is_blog());
    CHECK_FALSE(config.is_book());
}

TEST_CASE("Book") {
    Mock<IShell> shellMock;
    Config config(shellMock.get());

    char* argv[] = { (char*)"program", (char*)"book" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = config.parse(argc, argv);

    CHECK(result);
    CHECK(config.is_book());
    CHECK_FALSE(config.is_blog());
}

TEST_CASE("Both") {
    Mock<IShell> shellMock;
    Config config(shellMock.get());

    char* argv[] = { (char*)"program", (char*)"both" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool result = config.parse(argc, argv);

    CHECK(result);
    CHECK(config.is_book());
    CHECK(config.is_blog());
}

