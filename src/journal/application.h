#pragma once
#include "journal.h"

class Application {
public:
    Application(IConfig* config, IShell* shell, IGenerator* generator) : m_config(config), m_shell(shell), m_generator(generator) {}
    void run();
private:
    IConfig* m_config;
    IShell* m_shell;
    IGenerator* m_generator;
};
