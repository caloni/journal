#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class JournalEntry
{
public:
    void SetTitle(const std::string& title);
    void SetDate(const std::string& date);
    void SetLink(const std::string& link);
    void SetSlug(const std::string& slug);
    void SetTags(const std::string& tags);
    void SetContent(const std::string& content);

    const char* GetTitle() const;
    const char* GetDate() const;
    const char* GetLink() const;
    const char* GetSlug() const;
    const char* GetTags() const;
    const char* GetContent() const;

private:
    static std::string Slugify(const std::string& input);
    static char NormalizeChar(char c);

    std::string m_title; // title of the entry
    std::string m_date; // date in YYYY-MM-DD format
    std::string m_link; // external link for blogging
    std::string m_slug; // slugified title for URL
    std::string m_tags; // tags for the entry
    std::string m_content; // content of the entry after rendered
};

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

class Journal
{
public:
    size_t AddEntry();
    size_t EntryCount() const;
    bool IsValid() const;

    void SetEntryTitle(size_t entry, const std::string& title);
    void SetEntryDate(size_t entry, const std::string& date);
    void SetEntryLink(size_t entry, const std::string& link);
    void SetEntrySlug(size_t entry, const std::string& slug);
    void SetEntryTags(size_t entry, const std::string& tags);
    void SetEntryContent(size_t entry, const std::string& content);

    const char* GetEntryTitle(size_t entry) const;
    const char* GetEntryDate(size_t entry) const;
    const char* GetEntryLink(size_t entry) const;
    const char* GetEntrySlug(size_t entry) const;
    const char* GetEntryTags(size_t entry) const;
    const char* GetEntryContent(size_t entry) const;

    size_t GetEntryBySlug(const std::string& slug) const;

private:
    std::vector<JournalEntry> m_entries;
    std::vector<std::vector<std::shared_ptr<EntriesByTag>>> m_entriesTagsNavigation;
    std::vector<EntriesByDate> m_entriesByDate;
    std::vector<EntriesByMonth> m_entriesByMonth;
    std::unordered_map<std::string, std::vector<std::shared_ptr<EntriesByTag>>> m_entriesByTags;
    std::unordered_map<std::string, size_t> m_entriesBySlug;
    std::vector<PagesMonths> m_pagesMonths;
};

