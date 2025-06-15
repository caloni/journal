#pragma once
#include "ibook.h"
#include "ishell.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class Book : public IBook {
public:
    Book(IShell& shell) : m_shell(shell) {}
    int create(fs::path basedir, fs::path scriptdir, bool includeprivate = true);
private:
    IShell& m_shell;
};

