#pragma once
#include "blog.h"
#include "book.h"
#include "config.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class Journal {
public:
    Journal(Blog& blog, Book& book, Config& config) : m_blog(blog), m_book(book), m_config(config) {}
    void run();
private:
    Blog& m_blog;
    Book& m_book;
    Config& m_config;
};
