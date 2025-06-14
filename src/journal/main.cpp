#include "journal.h"
#include "blog.h"
#include "book.h"
#include <filesystem>
#include <fstream>

fs::path private_dir = fs::path("private");
fs::path private_repo = private_dir / ".git";

void print_usage() {
    std::cout << "Usage: journal command [options]\n";
    std::cout << "Command:\n";
    std::cout << "  blog             Build blog\n";
    std::cout << "  book             Build book\n";
    std::cout << "  both             Build both\n";
    std::cout << "Options:\n";
    std::cout << "  --publish          Publish content\n";
}

int main(int argc, char*argv[]) {
    if (argc == 1) {
        print_usage();
        return 0;
    }
    std::string command(argv[1]);
    bool blog = command == "blog" || command == "both";
    bool book = command == "book" || command == "both";
    if (!blog && !book) {
        print_usage();
        return 0;
    }
    bool publish = argc >= 3 && std::string(argv[2]) == "--publish";
    bool backup = publish;
    bool git_commit_push_private_repo = publish && fs::exists(private_repo) && fs::is_directory(private_repo);
    try {
        setup_encoding();
        fs::path basedir = fs::current_path();
        fs::path scriptdir = basedir / "src";
        std::cout << "basedir: " << basedir << '\n';
        fs::current_path(basedir);

        if (blog) {
            if (publish && backup) {
                create_backup(fs::current_path());
            }
            create_blog(basedir, scriptdir);

            if (git_commit_push_private_repo) {
                git_commit_push(private_dir, "Add journal changes");
            }

            git_commit_push(fs::current_path(), "Add journal changes");
            git_commit_push("public/blog", "Publish lastest changes");
        }
        if (book) {
            create_book(basedir, scriptdir);
        }

    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
