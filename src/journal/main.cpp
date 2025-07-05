//#include "journal.h"
#include <cmark.h>
#include <parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct JournalEntry
{
    void SetTitle(const std::string& title)
    {
        m_title = title;
    }
    void SetDocument(cmark_node* doc)
    {
        m_document = doc;
    }
    cmark_node* GetDocument() const
    {
        return m_document;
    }
    std::string GetTitle() const
    {
        return m_title;
    }
    void InsertMetadata(const std::string& key, const std::string& value)
    {
        m_metadata[key] = value;
    }
    std::string m_title;
    cmark_node* m_document;
    std::map<std::string, std::string> m_metadata;
};

struct Journal
{
    JournalEntry* AddEntry()
    {
        m_entries.push_back({ "", nullptr });
        return &m_entries.back();
    }
    size_t EntryCount() const
    {
        return m_entries.size();
    }
    JournalEntry* GetEntry(size_t index)
    {
        if (index < m_entries.size())
            return &m_entries[index];
        return nullptr;
    }
    std::vector<JournalEntry> m_entries;
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
void parse_kv_lines(std::string_view text, JournalEntry& entry)
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
            entry.InsertMetadata(std::string{key}, std::string{val});
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
    JournalEntry* current = nullptr;

    while (cur)
    {
        cmark_node* next = cmark_node_next(cur);   /* save before we maybe unlink */

        if (cmark_node_get_type(cur) == CMARK_NODE_HEADING && cmark_node_get_heading_level(cur) == 1)
        {
            current = journal.AddEntry();
            cmark_node* txt = cmark_node_first_child(cur);
            current->SetTitle(cmark_node_get_literal(txt));
            current->SetDocument(cmark_node_new(CMARK_NODE_DOCUMENT));

            if ((cur = cmark_node_next(cur)) && cmark_node_get_type(cur) == CMARK_NODE_PARAGRAPH)
            {
                txt = cmark_node_first_child(cur);
                while( txt )
                {
                    if (cmark_node_get_type(txt) == CMARK_NODE_TEXT && is_metadata_paragraph(txt))
                    {
                        parse_kv_lines(cmark_node_get_literal(txt), *current);
                    }
                    txt = cmark_node_next(txt);
                }
                next = cmark_node_next(cur);
                cmark_node_unlink(cur);
                cmark_node_free(cur);
            }
        }
        else if (current)
        {
            cmark_node_unlink(cur);
            cmark_node_append_child(current->GetDocument(), cur);
        }

        cur = next;
    }

    for (size_t i = 0; i < journal.EntryCount(); ++i)
    {
        cmark_iter* it = cmark_iter_new(journal.GetEntry(i)->GetDocument());
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

    for (size_t i = 0; i < journal.EntryCount(); ++i)
    {
        char* html = cmark_render_html(journal.GetEntry(i)->GetDocument(), CMARK_OPT_DEFAULT);
        printf("== %s ==\n%s\n\n", journal.GetEntry(i)->GetTitle().c_str(), html);
        free(html);
    }

}

