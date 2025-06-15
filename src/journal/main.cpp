#include "journal.h"
#include "blog.h"
#include "book.h"
#include "config.h"
#include <filesystem>
#include <fstream>

int main(int argc, char*argv[]) {
    try {
        Config config;
        if (!config.parse(argc, argv)) {
            config.print_usage();
            return 1;
        }
        Blog blog;
        Book book;
        Journal journal(blog, book, config);
        journal.run();
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
