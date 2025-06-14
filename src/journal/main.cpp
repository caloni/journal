#include "journal.h"
#include "blog.h"
#include "book.h"
#include <filesystem>
#include <fstream>

void print_usage() {
    std::cout << "Usage: journal command\n";
    std::cout << "Command:\n";
    std::cout << "  blog             Blog related\n";
    std::cout << "  book             Book related\n";
}

int main(int argc, char*argv[]) {
    if (argc == 1) {
        print_usage();
        return 0;
    }
    std::string command(argv[1]);
    if (command != "blog" && command != "book") {
        print_usage();
        return 0;
    }
    try {
        setup_encoding();
        fs::path basedir = fs::current_path();
        fs::path scriptdir = basedir / "src";
        std::cout << "basedir: " << basedir << '\n';
        fs::current_path(basedir);

        if (command == "blog") {
            create_blog(basedir, scriptdir);
        }
        else if (command == "book") {
            create_book(basedir, scriptdir);
        }

    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
