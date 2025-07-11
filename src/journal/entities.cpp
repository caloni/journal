#include "entities.h"
#include <algorithm>
#include <sstream>

void JournalEntry::SetTitle(const std::string& title)
{
    m_title = title;
    m_slug = Slugify(title);
}

void JournalEntry::SetDate(const std::string& date)
{
    m_date = date;
}

void JournalEntry::SetLink(const std::string& link)
{
    m_link = link;
}

void JournalEntry::SetSlug(const std::string& slug)
{
    m_slug = slug;
}

void JournalEntry::SetTags(const std::string& tags)
{
    m_tags = tags;
}

void JournalEntry::SetContent(const std::string& content)
{
    m_content = content;
}

const char* JournalEntry::GetTitle() const
{
    return m_title.c_str();
}

const char* JournalEntry::GetDate() const
{
    return m_date.c_str();
}

const char* JournalEntry::GetLink() const
{
    return m_link.c_str();
}

const char* JournalEntry::GetSlug() const
{
    return m_slug.c_str();
}

const char* JournalEntry::GetTags() const
{
    return m_tags.c_str();
}

const char* JournalEntry::GetContent() const
{
    return m_content.c_str();
}

std::string JournalEntry::Slugify(const std::string& input)
{
    std::string result;
    for (char c : input)
    {
        unsigned char norm = static_cast<unsigned char>(NormalizeChar(c));
        if (std::isalnum(norm))
        {
            result += norm;
        }
        else if (std::isspace(norm) || norm == '-')
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

char JournalEntry::NormalizeChar(char c)
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

struct EntriesByDate
{
    size_t date; // date as YYYY-MM-DD converted to size_t
    size_t entry; // index of the entry in the journal
};

struct EntriesByMonth
{
    size_t month; // month as YYYY-MM converted to size_t
    size_t end; // index of the last+1 entry in the month
};

struct PagesMonths
{
    size_t month; // month as YYYY-MM converted to size_t
    std::string html; // HTML content for the month page
};

struct EntriesByTag
{
    std::string tag; // tag name
    size_t entry; // index of the entry in the journal
    size_t next; // index of the next entry with the same tag
    size_t prev; // index of the previous entry with the same tag
};

size_t Journal::AddEntry()
{
    m_entries.push_back(JournalEntry());
    m_entriesTagsNavigation.push_back({});
    return m_entries.size() - 1;
}

size_t Journal::EntryCount() const
{
    return m_entries.size();
}

bool Journal::IsValid() const
{
    return m_entries.size() > 0
        && m_entriesByDate.size() > 0
        && m_entriesByMonth.size() > 0;
}

void Journal::SetEntryTitle(size_t entry, const std::string& title)
{
    if (entry >= m_entries.size()) return;
    m_entries[entry].SetTitle(title);
    m_entriesBySlug[m_entries[entry].GetSlug()] = entry;
}

void Journal::SetEntryDate(size_t entry, const std::string& date)
{
    if (entry >= m_entries.size()) return;
    m_entries[entry].SetDate(date);
    m_entriesByDate.push_back({ MakeDateKey(date), entry });
}

void Journal::SetEntryLink(size_t entry, const std::string& link)
{
    if (entry >= m_entries.size()) return;
    m_entries[entry].SetLink(link);
}

void Journal::SetEntrySlug(size_t entry, const std::string& slug)
{
    if (entry >= m_entries.size()) return;
    m_entries[entry].SetSlug(slug);
}

void Journal::SetEntryTags(size_t entry, const std::string& tags)
{
    if (entry >= m_entries.size()) return;
    m_entries[entry].SetTags(tags);
    SetEntryTagsNavigation(entry);
}

void Journal::SetEntryContent(size_t entry, const std::string& content)
{
    if (entry >= m_entries.size()) return;
    m_entries[entry].SetContent(content);
}

const char* Journal::GetEntryTitle(size_t entry) const
{
    return m_entries[entry].GetTitle().c_str();
}

const char* Journal::GetEntryDate(size_t entry) const
{
    return nullptr;
}

const char* Journal::GetEntryLink(size_t entry) const
{
    return nullptr;
}

const char* Journal::GetEntrySlug(size_t entry) const
{
    return nullptr;
}

const char* Journal::GetEntryTags(size_t entry) const
{
    return nullptr;
}

const char* Journal::GetEntryContent(size_t entry) const
{
    return nullptr;
}

size_t Journal::GetEntryBySlug(const std::string& slug) const
{
    return m_entriesBySlug.count(slug) ? m_entriesBySlug.at(slug) : static_cast<size_t>(-1);
}

