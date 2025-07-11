#pragma once
#include <filesystem>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

class JournalEntry
{
public:
    void set_title_and_slug(const std::string& title);
    void set_slug(const std::string& slug);
    void add_dates(const std::string& dates);
    void add_tags(const std::string& tags);
    void add_lines(const std::string& lines);

    std::string get_title() const;
    std::string get_slug() const;
    std::set<std::string> get_dates() const;
    std::set<std::string> get_tags() const;
    std::vector<std::string> get_lines() const;

    static bool is_valid_slug(const std::string& slug);

private:
    static std::string slugify(const std::string& input);
    static char normalize_char(char c);

    std::string m_slug;
    std::string m_title;
    std::set<std::string> m_dates;
    std::set<std::string> m_tags;
    std::vector<std::string> m_lines;
};

class Journal
{
public:
    void add_entry(const JournalEntry& entry);
    const std::vector<JournalEntry>& GetEntries();

private:
    std::vector<JournalEntry> m_entries;
};

