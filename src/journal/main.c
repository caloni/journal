#include <cmark.h>
#include <parser.h>
#include <ctype.h>

static char normalize_char(char c) {
    static const char* unnormalize_chars = "áàâäãéèêëíìîïóòôöõúùûüçñÁÀÂÄÃÉÈÊËÍÌÎÏÓÒÔÖÕÚÙÛÜÇÑ";
    static const char* normalize_chars = "aaaaaeeeeiiiiooooouuuucnaaaaaeeeeiiiiooooouuuucn";
    const char* uc = strchr(unnormalize_chars, c);
    if( uc ) {
        return normalize_chars[uc - unnormalize_chars];
    }
    return c;
}

static const char* slugify_title(const char* title) {
    char* slug = strdup(title);
    size_t len = strlen(title), i;
    char* start, * end;

    for ( i = 0 ; i < len; ++i ) {
        unsigned char c = normalize_char(slug[i]);
        if (isalnum(c)) {
            slug[i] = c;
        } else if (isspace(c) || c == '-') {
            slug[i] = '_';
        }
    }

    for (start = slug; *start && *start == '_'; ++start);
    for (end = slug + len - 1; end >= start && *end == '_'; --end);
    if (start > end) { slug[0] = '\0'; return slug; }
    memmove(slug, start, end - start + 1);
    return slug;
}

typedef struct journal_entry {
    const char* content;
    cmark_node* document;
    const char* date;
    const char* link;
    const char* slug;
    const char* tags;
    const char* title;
} journal_entry;

void parse_kv_lines(const char* text, journal_entry* entry) {
    char* sep = strchr(text, ':');
    if( sep ) {
        const char* value = sep + 2;
        *sep++ = '\0';
        if( strcmp(text, "date") == 0 ) {
            entry->date = _strdup(value);
        } else if( strcmp(text, "slug") == 0 ) {
            entry->slug = _strdup(value);
        } else if( strcmp(text, "link") == 0 ) {
            entry->link = _strdup(value);
        } else if( strcmp(text, "tags") == 0 ) {
            entry->tags = _strdup(value);
        }
    }
}

int main() {
    errno_t err = 0;
    FILE* fp = NULL;
    cmark_node* doc;
    size_t entry_count = 0;
    size_t entries_capacity = 1000;
    journal_entry* entries;
    cmark_node* cur;
    size_t i;

    if (!(entries = calloc(entries_capacity, sizeof(journal_entry)))) { perror("calloc"); return ENOMEM; }
    if ((err = fopen_s(&fp, "journal.md", "rb")) && !fp) { perror("fopen"); return err; }

    doc = cmark_parse_file(fp, 0);
    fclose(fp);

    cur = cmark_node_first_child(doc);
    while (cur) {
        cmark_node* next = cmark_node_next(cur);

        if (cmark_node_get_type(cur) == CMARK_NODE_HEADING && cmark_node_get_heading_level(cur) == 1) {

            if( ++entry_count > entries_capacity ) {
                journal_entry* old_entries = entries;
                size_t old_capacity = entries_capacity;
                entries_capacity *= 2;
                entries = (journal_entry*)realloc(entries, sizeof(journal_entry) * entries_capacity);
                if (entries == NULL) { perror("realloc"); return ENOMEM; }
                memset(entries + old_capacity, 0, sizeof(journal_entry) * (entries_capacity - old_capacity));
            }
            entries[entry_count-1].document = cmark_node_new(CMARK_NODE_DOCUMENT);
            assert(entries[entry_count-1].document != NULL);

            cmark_node* txt = cmark_node_first_child(cur);
            entries[entry_count-1].title = cmark_node_get_literal(txt);
            entries[entry_count-1].slug = slugify_title(entries[entry_count-1].title);

            if ((cur = cmark_node_next(cur)) && cmark_node_get_type(cur) == CMARK_NODE_PARAGRAPH) {

                txt = cmark_node_first_child(cur);
                while( txt ) {
                    if (cmark_node_get_type(txt) == CMARK_NODE_TEXT && strchr(cmark_node_get_literal(txt), ':')) {
                        parse_kv_lines(cmark_node_get_literal(txt), &entries[entry_count-1]);
                    }
                    txt = cmark_node_next(txt);
                }

                next = cmark_node_next(cur);
                cmark_node_unlink(cur);
                cmark_node_free(cur);
            }
        } else if (entry_count) {
            cmark_node_unlink(cur);
            cmark_node_append_child(entries[entry_count-1].document, cur);
        }

        cur = next;
    }

    for (i = 0; i < entry_count; ++i) {
        cmark_iter* it = cmark_iter_new(entries[i].document);
        cmark_event_type ev;

        while ((ev = cmark_iter_next(it)) != CMARK_EVENT_DONE) {
            cmark_node* nod;

            if (ev != CMARK_EVENT_ENTER) continue;
            nod = cmark_iter_get_node(it);
            if (cmark_node_get_type(nod) == CMARK_NODE_LINK)
            {
                const char* link = cmark_node_get_url(nod);
                /* todo find the entry by the link and replace it with permalink */
                /* cmark_node_set_url(nod, new_link); */
                /* todo check what to do with links that do not match any entry */

            } else if (cmark_node_get_type(nod) == CMARK_NODE_IMAGE) {

                const char* link = cmark_node_get_url(nod);
                size_t len = strlen(link), new_len = len + 5; // "img/" + link
                char* new_link = calloc(1, new_len);
                if( new_link ) {
                    strcpy_s("img/", new_len, new_link);
                    strcat_s(new_link, new_len, link);
                    cmark_node_set_url(nod, new_link);
                }
            }
        }
        cmark_iter_free(it);
    }

    for (i = 0; i < entry_count; ++i) {
        char* html = cmark_render_html(entries[i].document, CMARK_OPT_DEFAULT);
        entries[i].content = html;
    }
}

