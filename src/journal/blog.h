#pragma once
#include "iconfig.h"
#include "ioutput.h"
#include "ishell.h"

class Blog : public IOutput {
public:
    Blog(IConfig& config, IShell& shell) : m_config(config), m_shell(shell) {}
    int generate();
private:
    IConfig& m_config;
    IShell& m_shell;
};

