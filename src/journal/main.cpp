//#include "journal.h"
#include <cmark.h>
#include <parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <sstream>
#include <string>
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
<div class="is-4"><b>caloni::[entry_month]</b></div>
</a>
</div>
</div>
</nav>
<div class="container">
<div class="column">
<div style="min-height:56vh">
<div style="padding-bottom: 1em;"></div>
<ul style="list-style: none;">
[entry_index]
</ul>

[entry_content]

<span style="float: left;">
 <a href="[next_month].html">[next_month]</a>
 <a href="[previous_month].html">[previous_month]</a>
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
<li><small><a href="[month].html#[slug]">[title]</a></small></li>
)!!!!!";

constexpr char const * TEMPLATE_PAGES_MONTHS_CONTENT = R"!!!!!(
<span id="[slug]" title="[title]"/></span>
<section id="section_[slug]">
<p class="title"><a href="[month].html#[slug]">#</a>[title]</p>
<span class="title-heading">Caloni, [date]<a href="[month].html"> <sup>[up]</sup></a> <a href="javascript:;" onclick="copy_clipboard('section#section_about')"><sup>[copy]</sup></a></span>
[content]
</section><hr/>
)!!!!!";

struct JournalEntry
{
    cmark_node* m_document; // cmark document node for the entry
    std::string m_content; // content of the entry after rendered
    std::string m_date; // date in YYYY-MM-DD format
    std::string m_link; // external link for blogging
    std::string m_month; // month in YYYY-MM format
    std::string m_slug; // slugified title for URL
    std::string m_title; // title of the entry
    std::map<std::string, std::string> m_metadata; // metadata key-value pairs
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

struct Journal
{
    size_t AddEntry()
    {
        m_entries.push_back({ nullptr });
        return m_entries.size() - 1;
    }
    void Build()
    {
        SortAndIndex();
        BuildMonths();
        //BuildIndex()
        //BuildTags()
    }
    void SortAndIndex()
    {
        if( m_entries.empty() ) return;
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
    }
    void BuildMonths()
    {
        if (m_entriesByMonth.empty()) return;
        m_pagesMonths.push_back({ m_entriesByMonth[0].month, TEMPLATE_PAGES_MONTHS });
        std::string entries_index;
        std::string entries_content;
        size_t entryByDate = 0;
        for (auto& month : m_entriesByMonth)
        {
            while( entryByDate < month.end )
            {
                size_t entry = m_entriesByDate[entryByDate].entry;
                std::string new_entry_index = TEMPLATE_PAGES_MONTHS_INDEX;
                std::string new_entry_content;
                char* html = cmark_render_html(m_entries[entry].m_document, CMARK_OPT_DEFAULT);
                free(html);
                ++entry;
            }
        }
    }
    size_t EntryCount() const
    {
        return m_entries.size();
    }
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
    void SetEntryTitle(size_t entry, const std::string& title)
    {
        m_entries[entry].m_metadata["title"] = title;
        m_entries[entry].m_metadata["slug"] = Slugify(title);
    }
    void SetEntryDocument(size_t entry, cmark_node* doc)
    {
        m_entries[entry].m_document = doc;
    }
    cmark_node* GetEntryDocument(size_t entry) const
    {
        return m_entries[entry].m_document;
    }
    std::string GetEntryTitle(size_t entry) const
    {
        auto it = m_entries[entry].m_metadata.find("title");
        if (it != m_entries[entry].m_metadata.end())
        {
            return it->second;
        }
        return "";
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

    void InsertEntryMetadata(size_t entry, const std::string& key, const std::string& value)
    {
        m_entries[entry].m_metadata[key] = value;
        if( key == "date")
        {
            m_entriesByDate.push_back({ MakeDateKey(value), entry });
            m_entries[entry].m_metadata["month"] = value.substr(0, 7); // YYYY-MM
        }
    }
    std::vector<JournalEntry> m_entries;
    std::vector<EntriesByDate> m_entriesByDate;
    std::vector<EntriesByMonth> m_entriesByMonth;
    std::vector<PagesMonths> m_pagesMonths;
};

bool is_metadata_paragraph(cmark_node *n) {
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
    std::istringstream in{std::string{text}};  // getline needs a stream buffer
    std::string line;

    while (std::getline(in, line))
    {
        const std::size_t sep = line.find(':');
        if (sep == std::string::npos)          // no ':' → skip
            continue;

        std::string_view key = trim({line.data(), sep});
        std::string_view val = trim({line.data() + sep + 1, line.size() - sep - 1});

        if (!key.empty())
            journal.InsertEntryMetadata(entry, std::string{key}, std::string{val});
    }
}

int main()
{
    FILE* fp = nullptr;
    errno_t result = fopen_s(&fp, "journal.md", "r");
    if (!fp) { perror("fopen"); return 1; }
    cmark_node* doc = cmark_parse_file(fp, 0);
    fclose(fp);

    Journal journal;
    cmark_node* cur = cmark_node_first_child(doc);
    size_t entry = -1;

    while (cur)
    {
        cmark_node* next = cmark_node_next(cur);   /* save before we maybe unlink */

        if (cmark_node_get_type(cur) == CMARK_NODE_HEADING && cmark_node_get_heading_level(cur) == 1)
        {
            entry = journal.AddEntry();
            cmark_node* txt = cmark_node_first_child(cur);
            journal.SetEntryTitle(entry, cmark_node_get_literal(txt));
            journal.SetEntryDocument(entry, cmark_node_new(CMARK_NODE_DOCUMENT));

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
        else if (entry)
        {
            cmark_node_unlink(cur);
            cmark_node_append_child(journal.GetEntryDocument(entry), cur);
        }

        cur = next;
    }

    ///@todo rewrite links and images in all entries
    for (size_t i = 0; i < journal.EntryCount(); ++i)
    {
        cmark_iter* it = cmark_iter_new(journal.GetEntryDocument(i));
        while (cmark_iter_next(it) != CMARK_EVENT_DONE)
        {
            cmark_node* n = cmark_iter_get_node(it);

            if (cmark_node_get_type(n) == CMARK_NODE_LINK)
            {
                const char* dest = cmark_node_get_url(n);
                char* newdest = _strdup(dest); // todo rewrite_link(dest);      /* your function */
                if (newdest) cmark_node_set_url(n, newdest);
            }
            else if (cmark_node_get_type(n) == CMARK_NODE_IMAGE)
            {
                /* similar rewrite */
            }
        }
        cmark_iter_free(it);
    }

    journal.Build()

    /* sample on how to print the entries
    for (size_t i = 0; i < journal.EntryCount(); ++i)
    {
        char* html = cmark_render_html(journal.GetEntryDocument(i), CMARK_OPT_DEFAULT);
        printf("== %s ==\n%s\n\n", journal.GetEntryTitle(i).c_str(), html);
        free(html);
    }
    */
}

