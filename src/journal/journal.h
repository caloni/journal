#pragma once
#include "iblog.h"
#include "book.h"
#include "iconfig.h"
#include "ijournal.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class Journal : public IJournal {
public:
    Journal(IBlog& blog, IBook& book, IConfig& config, IShell& shell) : m_blog(blog), m_book(book), m_config(config), m_shell(shell) {}
    void run();
private:
    IBlog& m_blog;
    IBook& m_book;
    IConfig& m_config;
    IShell& m_shell;
};
