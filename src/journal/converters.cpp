#include "converters.h"

inline const std::string GetMonth(const std::string& date)
{
    return date.substr(0, 7); // YYYY-MM
}

inline const std::string GetPermalink(const std::string& date, const std::string& slug)
{
    return date.substr(0, 7) + std::string(".html#") + slug;
}

static inline std::string Journal::MonthFromMonthKey(size_t month)
{
    std::ostringstream os;
    os << month / 100 << '-' << std::setw(2) << std::setfill('0') << month % 100;
    return os.str();
}

/// Expect "YYYY-MM-DD" (10 characters)
inline size_t Journal::MakeDateKey(std::string_view date)
{
    // Allocate temporary buffer for 8-digit date string
    char buf[9] = {
        date[0], date[1], date[2], date[3], // YYYY
        date[5], date[6],                   // MM
        date[8], date[9],                   // DD
        '\0'                                // null terminator
    };
    return std::strtoull(buf, nullptr, 10);  // fast, simple, robust
}

void Journal::SetEntryTagsNavigation(size_t entry)
{
    std::stringstream sstags(m_entries[entry].GetTags());
    std::string tag;

    while (sstags >> tag)
    {
        auto tagNav = std::make_shared<EntriesByTag>();
        tagNav->tag = tag;
        tagNav->entry = entry;
        m_entriesTagsNavigation[entry].push_back(tagNav);
        m_entriesByTags[tag].push_back(tagNav);
    }
}

std::string SubstitutePlaceholders(std::string_view tmpl, const std::unordered_map<std::string_view, std::string_view>& dict)
{
    std::string result;
    result.reserve(tmpl.size());           // avoids reallocations

    std::size_t pos = 0;
    while (pos < tmpl.size())
    {
        auto open = tmpl.find('[', pos);
        if (open == std::string_view::npos)
        {   // no more markers
            result.append(tmpl.substr(pos));
            break;
        }

        result.append(tmpl.substr(pos, open - pos));  // literal chunk

        auto close = tmpl.find(']', open + 1);
        if (close == std::string_view::npos) {        // malformed ‘[’ with no ‘]’
            result.append(tmpl.substr(open));
            break;
        }

        auto key = tmpl.substr(open + 1, close - open - 1);
        if (auto it = dict.find(key); it != dict.end())
            result.append(it->second);                // substitute
        else
            result.append("[").append(key).append("]"); // leave untouched

        pos = close + 1;
    }
    return result;
}

void ConvertJournalToBlog(const Journal* journal)
{
    if (!journal->IsValid()) return;

    size_t entryByDate = 0;

    for (size_t i = 0; i < m_entriesByMonth.size(); ++i)
    {
        std::string entries_index;
        std::string entries_content;

        while (entryByDate < m_entriesByMonth[i].end)
        {
            size_t entry = m_entriesByDate[entryByDate].entry;

            std::unordered_map<std::string_view, std::string_view> vars;
            vars["title"] = m_entries[entry].GetTitle(); // title of the entry
            vars["date"] = m_entries[entry].GetDate(); // date in YYYY-MM-DD format
            vars["link"] = m_entries[entry].GetLink(); // external link for blogging
            vars["slug"] = m_entries[entry].GetSlug(); // slugified title for URL
            vars["tags"] = m_entries[entry].GetTags(); // tags for the entry
            vars["content"] = m_entries[entry].GetContent(); // content of the entry after rendered
            std::string month = m_entries[entry].GetMonth(); // month in YYYY-MM format
            vars["month"] = month;
            std::string permalink = m_entries[entry].GetPermalink(); // permalink for the entry
            vars["permalink"] = permalink;

            std::string index = TEMPLATE_PAGES_MONTHS_INDEX;
            auto out = SubstitutePlaceholders(index, vars);
            entries_index += out;
            std::string tags_nav_out;
            for (auto& tag : m_entriesTagsNavigation[entry])
            {
                std::string prevPermalink = m_entries[tag->prev].GetPermalink();
                std::string nextPermalink = m_entries[tag->next].GetPermalink();
                std::unordered_map<std::string_view, std::string_view> tag_vars{
                    { "tag", tag->tag },
                    { "previous_in_tag_permalink", prevPermalink },
                    { "next_in_tag_permalink", nextPermalink },
                };
                std::string tags_nav = TEMPLATE_PAGES_ENTRY_TAGS_HEADER;
                out = SubstitutePlaceholders(tags_nav, tag_vars);
                tags_nav_out += out;
            }
            vars["tags_nav"] = tags_nav_out;
            std::string content = TEMPLATE_PAGES_MONTHS_CONTENT;
            out = SubstitutePlaceholders(content, vars);
            entries_content += out;

            ++entryByDate;
        }

        auto lastEntry = m_entriesByDate[entryByDate - 1].entry;
        auto monthS = MonthFromMonthKey(m_entriesByMonth[i].month);
        auto nextMonth = i + 1 == m_entriesByMonth.size() ? 0 : i + 1;
        auto previousMonth = i == 0 ? m_entriesByMonth.size() - 1 : i - 1;
        auto nextMonthS = MonthFromMonthKey(m_entriesByMonth[nextMonth].month);
        auto previousMonthS = MonthFromMonthKey(m_entriesByMonth[previousMonth].month);;
        std::string page = TEMPLATE_PAGES_MONTHS;
        std::unordered_map<std::string_view, std::string_view> vars{
            { "index",  entries_index },
            { "content", entries_content },
            { "month", monthS },
            { "next_month", nextMonthS },
            { "previous_month", previousMonthS },
        };
        auto out = SubstitutePlaceholders(page, vars);
        m_pagesMonths.push_back({ m_entriesByMonth[i].month, out });
        std::ofstream ofs("public/blog/" + monthS + ".html", std::ios::binary);
        if (ofs)
        {
            ofs.write(out.c_str(), out.size());
        }
    }
}

