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

