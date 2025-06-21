#include "blog.h"
#include "book.h"
#include "config.h"
#include "application.h"
#include "shell.h"
#include <filesystem>
#include <fstream>

int main(int argc, char*argv[]) {
    try {
        Shell shell;
        Config config(shell);
        if (!config.parse(argc, argv)) {
            config.print_usage();
            return 1;
        }
        Blog blog(config, shell);
        Book book(config, shell);
        Application application(blog, book, config, shell);
        application.run();
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
