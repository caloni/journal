#include "doctest.h"
#include "fakeit.hpp"
#include "..\journal.h"

using namespace fakeit;

TEST_CASE("Add a new journal entry")
{
    const auto title = "My first entry";
    const auto expected_title = title;
    const auto expected_slug = "my_first_entry";
    const auto dates = "2023-10-01";
    const auto expected_dates = std::set<std::string>{ "2023-10-01" };
    const auto tags = "journal blogging";
    const auto expected_tags = std::set<std::string>{ "journal", "blogging" };
    const auto lines = R"(This is my first journal entry.
I am excited to start this journey!
I hope to write every day.)";
    const auto expected_lines = std::vector<std::string>{
        "This is my first journal entry.",
        "I am excited to start this journey!",
        "I hope to write every day."
    };

    JournalEntry sut;
    sut.set_title_and_slug(title);
    sut.add_dates(dates);
    sut.add_tags(tags);
    sut.add_lines(lines);

    CHECK(sut.get_title() == expected_title);
    CHECK(sut.get_slug() == expected_slug);
    CHECK(sut.get_dates() == expected_dates);
    CHECK(sut.get_tags() == expected_tags);
    CHECK(sut.get_lines() == expected_lines);
}

