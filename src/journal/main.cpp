#include "blog.h"
#include "book.h"
#include "config.h"
#include "journal.h"
#include "shell.h"
#include <filesystem>
#include <fstream>

int main(int argc, char*argv[]) {
    try {
        Config config;
        if (!config.parse(argc, argv)) {
            config.print_usage();
            return 1;
        }
        Shell shell;
        Blog blog(shell);
        Book book(shell);
        Journal journal(blog, book, config, shell);
        journal.run();
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
