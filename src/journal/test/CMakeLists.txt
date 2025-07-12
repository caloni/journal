cmake_minimum_required(VERSION 3.7)

add_executable(journal_test harness.c harness.h main.c)

target_link_libraries(journal_test PRIVATE libjournal)

add_test(NAME journal_test COMMAND journal_test)

