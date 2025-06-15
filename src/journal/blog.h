#pragma once
#include "iblog.h"
#include "ishell.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class Blog : public IBlog {
public:
    Blog(IShell& shell) : m_shell(shell) {}
    int create(fs::path basedir, fs::path scriptdir);
private:
    IShell& m_shell;
};

