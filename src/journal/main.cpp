//#include "journal.h"
#include <cmark.h>

#include <parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

constexpr char const * TEMPLATE_PAGES_MONTHS = R"!!!!!(
<!DOCTYPE html>
<html lang="en-us" dir="ltr" itemscope itemtype="http://schema.org/Article">
<head>
<meta charset="utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1"/>
<title>Blogue do Caloni</title>
<meta name="author" content="Caloni" />
<meta name="generator" content="https://github.com/caloni/journal">
<meta property="og:title" content="Blogue do Caloni"/>
<meta property="og:type" content="website"/>
<meta property="og:url" content="http://www.caloni.com.br"/>
<meta property="og:image" content="/img/about-brand.png"/>
<meta property="og:description" content="Write for computers, people and food."/>
<link href="/index.xml" rel="feed" type="application/rss+xml" title="Blogue do Caloni"/>
<link rel="stylesheet" type="text/css" href="/css/custom.css"/>
<link rel="stylesheet" type="text/css" href="/css/jquery-ui.css"/>
<script src="/js/jquery-1.12.4.js"></script>
<script src="/js/jquery-ui.js"></script>
<script src="/js/copy_clipboard.js"></script>
<script>
var quick_search_posts = [ 
 ]; 
</script>
<script src="/js/quick_search.js"></script>
<script src="/js/list.js"></script>
<link rel="icon" href="/img/favicon.ico"/>
</head>
<body style="min-height:100vh;display:flex;flex-direction:column">
<nav class="navbar has-shadow is-white"
role="navigation" aria-label="main navigation">
<div class="container">
<div class="navbar-brand">
&nbsp;
<a class="navbar-item" href="months.html">
<div class="is-4"><b>caloni::[month]</b></div>
</a>
</div>
</div>
</nav>
<div class="container">
<div class="column">
<div style="min-height:56vh">
<div style="padding-bottom: 1em;"></div>
<ul style="list-style: none;">
[index]
</ul>

[content]

<span style="float: left;">
 <a href="[previous_month].html">&lt; </a>
 <a href="months.html">months</a>
 <a href="[next_month].html"> &gt;</a>
</span>
</div>
</div>
</section>
<footer class="footer">
<div class="container">
</div>
<div class="intentionally-blank"></div>
</footer>
</body>
</html>
)!!!!!";

constexpr char const * TEMPLATE_PAGES_MONTHS_INDEX = R"!!!!!(
<li><small><a href="[permalink]">[title]</a></small></li>
)!!!!!";

constexpr char const * TEMPLATE_PAGES_ENTRY_TAGS_HEADER = R"!!!!!(
<a href="[previous_in_tag_permalink]">&lt;</a><a href="[tag].html">[tag]</a><a href="[next_in_tag_permalink]">&gt;</a> 
)!!!!!";

constexpr char const * TEMPLATE_PAGES_MONTHS_CONTENT = R"!!!!!(
<span id="[slug]" title="[title]"/></span>
<section id="section_[slug]">
<p class="title"><a href="[month].html#[slug]">#</a> [title]</p>
<span class="title-heading">Caloni, [date][tags_nav]<a href="[month].html"> <sup>[up]</sup></a> <a href="javascript:;" onclick="copy_clipboard('section#section_[slug]')"><sup>[copy]</sup></a></span>
[content]
</section><hr/>
)!!!!!";

class JournalEntry
{
public:
    void SetTitle(const std::string& title)
    {
        m_title = title;
        m_slug = Slugify(title);
    }

    void SetDate(const std::string& date)
    {
        m_date = date;
    }

    void SetLink(const std::string& link)
    {
        m_link = link;
    }

    void SetSlug(const std::string& slug)
    {
        m_slug = slug;
    }

    void SetTags(const std::string& tags)
    {
        m_tags = tags;
    }

    void SetContent(const std::string& content)
    {
        m_content = content;
    }

    const std::string& GetTitle() const
    {
        return m_title;
    }

    const std::string& GetDate() const
    {
        return m_date;
    }

    const std::string& GetLink() const
    {
        return m_link;
    }

    const std::string& GetSlug() const
    {
        return m_slug;
    }

    const std::string& GetTags() const
    {
        return m_tags;
    }

    const std::string& GetContent() const
    {
        return m_content;
    }

    std::string GetMonth() const
    {
        return m_date.substr(0, 7); // YYYY-MM
    }

    std::string GetPermalink() const
    {
        return m_date.substr(0, 7) + std::string(".html#") + m_slug;
    }

private:
    static std::string Slugify(const std::string& input)
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

    static char NormalizeChar(char c)
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
    size_t AddEntry()
    {
        m_entries.push_back(JournalEntry());
        m_entriesTagsNavigation.push_back({});
        return m_entries.size() - 1;
    }

    size_t EntryCount() const
    {
        return m_entries.size();
    }

    void SetEntryTitle(size_t entry, const std::string& title)
    {
        if (entry >= m_entries.size()) return;
        m_entries[entry].SetTitle(title);
        m_entriesBySlug[m_entries[entry].GetSlug()] = entry;
    }

    void SetEntryDate(size_t entry, const std::string& date)
    {
        if (entry >= m_entries.size()) return;
        m_entries[entry].SetDate(date);
        m_entriesByDate.push_back({ MakeDateKey(date), entry });
    }

    void SetEntryLink(size_t entry, const std::string& link)
    {
        if (entry >= m_entries.size()) return;
        m_entries[entry].SetLink(link);
    }

    void SetEntrySlug(size_t entry, const std::string& slug)
    {
        if (entry >= m_entries.size()) return;
        m_entries[entry].SetSlug(slug);
    }

    void SetEntryTags(size_t entry, const std::string& tags)
    {
        if (entry >= m_entries.size()) return;
        m_entries[entry].SetTags(tags);
        SetEntryTagsNavigation(entry);
    }

    void SetEntryContent(size_t entry, const std::string& content)
    {
        if (entry >= m_entries.size()) return;
        m_entries[entry].SetContent(content);
    }

    std::string GetEntryPermalink(const std::string& slug) const
    {
        auto it = m_entriesBySlug.find(slug);
        if (it != m_entriesBySlug.end())
        {
            return m_entries[it->second].GetPermalink();
        }
        return {};
    }

    void Index()
    {
        if( m_entries.empty() || m_entriesByDate.empty() ) return;
        std::stable_sort(m_entriesByDate.begin(), m_entriesByDate.end(),
                 [](const EntriesByDate& a, const EntriesByDate& b){ return a.date < b.date; });
        m_entriesByMonth.push_back({ m_entriesByDate[0].date / 100, 1 });
        for (size_t i = 1; i < m_entriesByDate.size(); ++i)
        {
            if (m_entriesByDate[i].date / 100 != m_entriesByMonth.back().month)
            {
                m_entriesByMonth.push_back({ m_entriesByDate[i].date / 100, i + 1 });
            }
            else
            {
                m_entriesByMonth.back().end = i + 1;
            }
        }
        for (auto& tags : m_entriesByTags)
        {
            std::stable_sort(tags.second.begin(), tags.second.end(),
                [this](const std::shared_ptr<EntriesByTag>& a, const std::shared_ptr<EntriesByTag>& b)
                { return m_entriesByDate[a->entry].date < m_entriesByDate[b->entry].date; });
            int x = 0;
            for (size_t i = 0; i < tags.second.size(); ++i)
            {
                tags.second[i]->prev = (i > 0) ? tags.second[i - 1]->entry : tags.second[tags.second.size() - 1]->entry;
                tags.second[i]->next = (i + 1 < tags.second.size()) ? tags.second[i + 1]->entry : tags.second[0]->entry;
            }
            x = x + 1;
        }
    }

    void Build()
    {
        if (m_entriesByMonth.empty()) return;

        size_t entryByDate = 0;

        for( size_t i = 0; i < m_entriesByMonth.size(); ++i )
        {
            std::string entries_index;
            std::string entries_content;

            while( entryByDate < m_entriesByMonth[i].end )
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

            auto lastEntry = m_entriesByDate[entryByDate-1].entry;
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
            if( ofs )
            {
                ofs.write(out.c_str(), out.size());
            }
        }
    }

    private:
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

    /// Expect "YYYY-MM-DD" (10 characters)
    static inline size_t MakeDateKey(std::string_view date)
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

    static inline std::string MonthFromMonthKey(size_t month)
    {
        std::ostringstream os;
        os << month / 100 << '-' << std::setw(2) << std::setfill('0') << month % 100;
        return os.str();
    }

    void SetEntryTagsNavigation(size_t entry)
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

    std::vector<JournalEntry> m_entries;
    std::vector<std::vector<std::shared_ptr<EntriesByTag>>> m_entriesTagsNavigation;
    std::vector<EntriesByDate> m_entriesByDate;
    std::vector<EntriesByMonth> m_entriesByMonth;
    std::unordered_map<std::string, std::vector<std::shared_ptr<EntriesByTag>>> m_entriesByTags;
    std::unordered_map<std::string, size_t> m_entriesBySlug;
    std::vector<PagesMonths> m_pagesMonths;
};

bool is_metadata_paragraph(cmark_node *n)
{
    const char *text = cmark_node_get_literal(n);
    char *copy = _strdup(text);
    char *next_token = NULL;
    char *line = strtok_s(copy, "\n", &next_token);
    while (line) {
        while (*line == ' ') line++;  // trim leading spaces
        if (!strchr(line, ':')) {
            free(copy);
            return false;  // line missing ':' → not metadata
        }
        line = strtok_s(NULL, "\n", &next_token);
    }
    free(copy);
    return true;
}

/// Remove leading and trailing ASCII whitespace (space, tab, CR, LF).
static std::string_view trim(std::string_view sv)
{
    const auto first = sv.find_first_not_of(" \t\r\n");
    if (first == std::string_view::npos)       // all whitespace
        return {};

    const auto last  = sv.find_last_not_of(" \t\r\n");
    return sv.substr(first, last - first + 1);
}

/// Parse “key: value” pairs separated by new‑lines and feed them to JournalEntry.
void parse_kv_lines(std::string_view text, Journal& journal, size_t entry)
{
    typedef void (Journal::*SetEntryPropertyMethod)(size_t entry, const std::string& title);

    const std::unordered_map<std::string, SetEntryPropertyMethod> JournalEntryProperties = {
        { "title", &Journal::SetEntryTitle },
        { "date", &Journal::SetEntryDate },
        { "link", &Journal::SetEntryLink },
        { "slug", &Journal::SetEntrySlug },
        { "tags", &Journal::SetEntryTags }
    };

    std::istringstream in{std::string{text}};  // getline needs a stream buffer
    std::string line;

    while (std::getline(in, line))
    {
        const std::size_t sep = line.find(':');
        if (sep == std::string::npos) // no ':' → skip
            continue;

        std::string_view key = trim({line.data(), sep});
        std::string_view val = trim({line.data() + sep + 1, line.size() - sep - 1});

        auto it = JournalEntryProperties.find(std::string{ key });
        if (it != JournalEntryProperties.end())
        {
            SetEntryPropertyMethod method = it->second;
            (journal.*method)(entry, std::string{ val });
            continue;
        }
    }
}

int main()
{
    FILE* fp = nullptr;
    errno_t result = fopen_s(&fp, "journal.md", "rb");
    if (!fp) { perror("fopen"); return 1; }
    cmark_node* doc = cmark_parse_file(fp, 0);
    fclose(fp);

    Journal journal;
    std::vector<cmark_node*> entriesDocument; // cmark document node for the journal entries
    cmark_node* cur = cmark_node_first_child(doc);
    size_t entry = -1;

    // separate journal entries from total document
    while (cur)
    {
        cmark_node* next = cmark_node_next(cur); // save before we maybe unlink

        if (cmark_node_get_type(cur) == CMARK_NODE_HEADING && cmark_node_get_heading_level(cur) == 1)
        {
            entry = journal.AddEntry();
            entriesDocument.push_back(cmark_node_new(CMARK_NODE_DOCUMENT));
            cmark_node* txt = cmark_node_first_child(cur);
            journal.SetEntryTitle(entry, cmark_node_get_literal(txt));

            if ((cur = cmark_node_next(cur)) && cmark_node_get_type(cur) == CMARK_NODE_PARAGRAPH)
            {
                txt = cmark_node_first_child(cur);
                while( txt )
                {
                    if (cmark_node_get_type(txt) == CMARK_NODE_TEXT && is_metadata_paragraph(txt))
                    {
                        parse_kv_lines(cmark_node_get_literal(txt), journal, entry);
                    }
                    txt = cmark_node_next(txt);
                }
                next = cmark_node_next(cur);
                cmark_node_unlink(cur);
                cmark_node_free(cur);
            }
        }
        else if (journal.EntryCount())
        {
            cmark_node_unlink(cur);
            cmark_node_append_child(entriesDocument[entry], cur);
        }

        cur = next;
    }

    // adjust links and images
    for (size_t i = 0; i < journal.EntryCount(); ++i)
    {
        cmark_iter* it = cmark_iter_new(entriesDocument[i]);
        cmark_event_type ev;
        while ((ev = cmark_iter_next(it)) != CMARK_EVENT_DONE)
        {
            if (ev != CMARK_EVENT_ENTER) // pass only once
                continue;
            cmark_node* n = cmark_iter_get_node(it);

            if (cmark_node_get_type(n) == CMARK_NODE_LINK)
            {
                std::string dest = cmark_node_get_url(n);
                std::string newDest = journal.GetEntryPermalink(dest);
                if (!newDest.empty())
                {
                    cmark_node_set_url(n, newDest.c_str());
                }
                else
                {
                    /// @todo check what to do with links that do not match any entry
                }
            }
            else if (cmark_node_get_type(n) == CMARK_NODE_IMAGE)
            {
                const char* dest = cmark_node_get_url(n);
                auto newdest = std::string("img/") + dest;
                cmark_node_set_url(n, newdest.c_str());
            }
        }
        cmark_iter_free(it);
    }

    // render entries to HTML
    for (size_t entry = 0; entry < journal.EntryCount(); ++entry)
    {
        char* html = cmark_render_html(entriesDocument[entry], CMARK_OPT_DEFAULT);
        journal.SetEntryContent(entry, html);
        free(html);
    }

    journal.Index(); // mount indexes
    journal.Build(); // build blog
}

