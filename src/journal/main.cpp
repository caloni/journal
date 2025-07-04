#include "journal.h"
#include <cmark.h>
#include <parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <string>
#include <vector>

typedef struct {
    char* title;            // from the H1 literal
    cmark_node* body_root;  // subtree with metadata + H1 removed
    std::map<std::string, std::string> *meta;          // your own struct/map for key=value pairs
} entry_t;

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

void parse_kv_lines(const char *text, std::map<std::string, std::string>& kv) {
    char *copy = _strdup(text);
    char *next_token = NULL;
    char *line = strtok_s(copy, "\n", &next_token);
    while (line) {
        char *sep = strchr(line, ':');
        if (sep) {
            *sep = '\0';
            const char *key = line; // todo trim(line);
            const char *val = sep + 1; //todo trim(sep + 1);
            //kv_insert(kv, key, val);
            kv[key] = val;
        }
        line = strtok_s(NULL, "\n", &next_token);
    }
    free(copy);
}

int main()
{
    FILE* fp = nullptr;
    errno_t result = fopen_s(&fp, "journal.md", "r");
    cmark_node* doc = cmark_parse_file(fp, 0);
    fclose(fp);

    entry_t* entries = NULL;
    size_t    n_entries = 0;

    cmark_node* cur = cmark_node_first_child(doc);
    entry_t* current = NULL;

    while (cur) {
        cmark_node* next = cmark_node_next(cur);   /* save before we maybe unlink */

        /* 3.a – separator: level‑1 heading → start new entry */
        if (cmark_node_get_type(cur) == CMARK_NODE_HEADING &&
            cmark_node_get_heading_level(cur) == 1)
        {
            /* Finish previous entry if any */
            current = (entry_t*)realloc(entries, ++n_entries * sizeof(entry_t));
            entries = (entry_t*)current;           /* keep pointer stable */
            current = &entries[n_entries-1];
            current->meta = new std::map<std::string, std::string>();

            /* Harvest title text */
            cmark_node* txt = cmark_node_first_child(cur);
            current->title = _strdup(cmark_node_get_literal(txt));

            /* Create container for body nodes */
            current->body_root = cmark_node_new(CMARK_NODE_DOCUMENT);

            cur = cmark_node_next(cur);

            /* Look ahead to possible metadata paragraph */
            if (cur && cmark_node_get_type(cur) == CMARK_NODE_PARAGRAPH) {
                txt = cmark_node_first_child(cur);
                while( txt ) {
                    if (cmark_node_get_type(txt) == CMARK_NODE_TEXT && is_metadata_paragraph(txt)) {
                        parse_kv_lines(cmark_node_get_literal(txt), *current->meta);
                    }
                    txt = cmark_node_next(txt);
                }
                next = cmark_node_next(cur);
                cmark_node_unlink(cur);
                cmark_node_free(cur);
            }
        }
        /* 3.c – normal content node → move into body_root */
        else if (current) {
            cmark_node_unlink(cur);
            cmark_node_append_child(current->body_root, cur);
        }

        cur = next;
    }

    for (size_t i = 0; i < n_entries; ++i) {
        cmark_iter* it = cmark_iter_new(entries[i].body_root);
        while (cmark_iter_next(it) != CMARK_EVENT_DONE) {
            cmark_node* n = cmark_iter_get_node(it);

            if (cmark_node_get_type(n) == CMARK_NODE_LINK) {
                const char* dest = cmark_node_get_url(n);
                char* newdest = _strdup(dest); // todo rewrite_link(dest);      /* your function */
                if (newdest) cmark_node_set_url(n, newdest);
            }
            else if (cmark_node_get_type(n) == CMARK_NODE_IMAGE) {
                /* similar rewrite */
            }
        }
        cmark_iter_free(it);
    }

    for (size_t i = 0; i < n_entries; ++i) {
        char* html = cmark_render_html(entries[i].body_root, CMARK_OPT_DEFAULT);
        printf("== %s ==\n%s\n\n", entries[i].title, html);
        free(html);
    }

}

