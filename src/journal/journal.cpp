#include "journal.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <unordered_map>

void JournalEntry::set_title_and_slug(const std::string& title)
{
    m_title = title;
    m_slug = slugify(title);
}

void JournalEntry::set_slug(const std::string& slug)
{
    if (!is_valid_slug(slug))
    {
        throw std::exception("Invalid slug: Slugs can only contain alphanumeric characters and underscores.");
    }
    m_slug = slugify(slug);
}

void JournalEntry::add_dates(const std::string& dates)
{
    std::set<std::string> new_dates;
    std::istringstream iss(dates);
    std::string date;
    while (iss >> date)
    {
        new_dates.insert(date);
    }
    m_dates.insert(new_dates.begin(), new_dates.end());
}

void JournalEntry::add_tags(const std::string& tags)
{
    std::set<std::string> new_tags;
    std::istringstream iss(tags);
    std::string tag;
    while (iss >> tag)
    {
        new_tags.insert(tag);
    }
    m_tags.insert(new_tags.begin(), new_tags.end());
}

void JournalEntry::add_lines(const std::string& lines)
{
    std::vector<std::string> new_lines;
    std::istringstream iss(lines);
    std::string line;
    while (std::getline(iss, line))
    {
        new_lines.push_back(line);
    }
    m_lines.insert(m_lines.end(), new_lines.begin(), new_lines.end());
}

std::string JournalEntry::get_title() const
{
    return m_title;
}

std::string JournalEntry::get_slug() const
{
    return m_slug;
}

std::set<std::string> JournalEntry::get_dates() const
{
    return m_dates;
}

std::set<std::string> JournalEntry::get_tags() const
{
    return m_tags;
}

std::vector<std::string> JournalEntry::get_lines() const
{
    return m_lines;
}

bool JournalEntry::is_valid_slug(const std::string& slug)
{
    for (const char& c : slug)
    {
        if (!std::isalnum(c) && c != '_')
        {
            return false;
        }
    }
    return true;
}

void Journal::add_entry(const JournalEntry& entry)
{
    m_entries.push_back(entry);
}

std::string JournalEntry::slugify(const std::string& input)
{
    std::string result;
    for (char c : input)
    {
        char norm = normalize_char(c);
        if (std::isalnum(norm))
        {
            result += norm;
        }
        else if (std::isspace(c) || c == '-')
        {
            result += '_';
        }
    }

    auto start = result.find_first_not_of('_');
    auto end = result.find_last_not_of('_');
    if (start == std::string::npos)
        return "";

    return result.substr(start, end - start + 1);
}

char JournalEntry::normalize_char(char c)
{
    static const std::unordered_map<char, char> accent_map = {
        {'á','a'}, {'à','a'}, {'â','a'}, {'ä','a'}, {'ã','a'},
        {'é','e'}, {'è','e'}, {'ê','e'}, {'ë','e'},
        {'í','i'}, {'ì','i'}, {'î','i'}, {'ï','i'},
        {'ó','o'}, {'ò','o'}, {'ô','o'}, {'ö','o'}, {'õ','o'},
        {'ú','u'}, {'ù','u'}, {'û','u'}, {'ü','u'},
        {'ç','c'},
        {'ñ','n'},
        {'Á','a'}, {'À','a'}, {'Â','a'}, {'Ä','a'}, {'Ã','a'},
        {'É','e'}, {'È','e'}, {'Ê','e'}, {'Ë','e'},
        {'Í','i'}, {'Ì','i'}, {'Î','i'}, {'Ï','i'},
        {'Ó','o'}, {'Ò','o'}, {'Ô','o'}, {'Ö','o'}, {'Õ','o'},
        {'Ú','u'}, {'Ù','u'}, {'Û','u'}, {'Ü','u'},
        {'Ç','c'},
        {'Ñ','n'},
    };

    auto it = accent_map.find(c);
    if (it != accent_map.end())
    {
        return it->second;
    }
    return std::tolower(static_cast<unsigned char>(c));
}

