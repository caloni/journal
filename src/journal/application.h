#pragma once
#include "iconfig.h"
#include "ioutput.h"
#include "ishell.h"

class Application {
public:
    Application(IOutput& blog, IOutput& book, IConfig& config, IShell& shell) : m_blog(blog), m_book(book), m_config(config), m_shell(shell) {}
    void run();
private:
    IOutput& m_blog;
    IOutput& m_book;
    IConfig& m_config;
    IShell& m_shell;
};
