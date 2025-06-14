#include "journal.h"
#include "book.h"
#include <fstream>

int create_book(fs::path basedir, fs::path scriptdir) {
    std::string build_version = run_command("git rev-parse --short HEAD");
    std::string current_date = current_datetime();

    fs::path book_public = basedir / "public" / "book";
    if (!fs::exists(book_public)) {
        fs::create_directories(book_public);
    }

    clear_directory(book_public);

    fs::copy(basedir / "book", book_public, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
    fs::copy(basedir / "img" / "book", book_public / "EPUB" / "img", fs::copy_options::recursive | fs::copy_options::overwrite_existing);
    fs::copy(basedir / "img" / "blog", book_public / "EPUB" / "img", fs::copy_options::recursive | fs::copy_options::overwrite_existing);

    // Run awk scripts
    std::string util_awk = (scriptdir / "Util.awk").string();
    std::string meta_awk = (scriptdir / "MetadataParser.awk").string();
    std::string writer_awk = (scriptdir / "MetadataWriter.awk").string();
    std::string md_file = "journal.md";

    std::string cmd = "gawk -f \"" + util_awk + "\" -f \"" + meta_awk + "\" -f \"" + writer_awk + "\" \"" + md_file + "\"";
    int ret = std::system(cmd.c_str());
    if (ret != 0) std::cerr << "MetadataWriter.awk returned " << ret << '\n';

    // Append metadata
    {
        std::ofstream metadata((basedir / "public" / "metadata.txt").string(), std::ios::app | std::ios::binary);
        metadata << "metadata_current_date " << current_date << "\n";
        metadata << "metadata_build_version " << build_version << "\n";
    }

    // Run second awk command
    std::string parser_awk = (scriptdir / "MarkdownParser.awk").string();
    std::string bookwriter_awk = (scriptdir / "BookWriter.awk").string();
    std::string metadata_file = (basedir / "public" / "metadata.txt").string();

    cmd = "gawk -f \"" + util_awk + "\" -f \"" + parser_awk + "\" -f \"" + bookwriter_awk + "\" \"" + metadata_file + "\" \"" + md_file + "\"";
    ret = std::system(cmd.c_str());
    if (ret != 0) std::cerr << "txt2book.awk returned " << ret << '\n';

    // Pack the book
    fs::current_path(book_public);
    cmd = "python repack.py";
    ret = std::system(cmd.c_str());
    if (ret != 0) std::cerr << "repack.py returned " << ret << '\n';
    fs::current_path(basedir);

    std::cout << "book generated\n";

    return 0;
}
