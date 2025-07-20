#include <cmark.h>
#include <parser.h>
#include <ctype.h>

typedef struct journal_entry {
    const char* content;
    cmark_node* document;
    const char* date;
    const char* link;
    const char* slug;
    const char* tags;
    const char* title;
} journal_entry;

typedef struct journal {
    journal_entry* entries;
    size_t entries_capacity;
    size_t entries_count;
    size_t* entries_by_date;
} journal;

static journal g_journal;

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

void parse_kv_lines(const char* text, journal_entry* entry) {
    char* sep = strchr(text, ':');
    if( sep ) {
        const char* value = sep + 2;
        *sep++ = '\0';
        if( strcmp(text, "date") == 0 ) {
            entry->date = strdup(value);
        } else if( strcmp(text, "slug") == 0 ) {
            entry->slug = strdup(value);
        } else if( strcmp(text, "link") == 0 ) {
            entry->link = strdup(value);
        } else if( strcmp(text, "tags") == 0 ) {
            entry->tags = strdup(value);
        }
    }
}

long long date_to_number(const char* date) {
    char buf[9];
    if( date == NULL ) return UINT_MAX;
    buf[0] = date[0], buf[1] = date[1], buf[2] = date[2], buf[3] = date[3]; /* YYYY */
    buf[4] = date[5], buf[5] = date[6]; /* MM */
    buf[6] = date[8], buf[7] = date[9]; /* DD */
    buf[8] = '\0';
    return strtoull(buf, NULL, 10);
}

int compare_entries_by_date(const void* a, const void* b) {
    size_t entryA = *(size_t*)a;
    size_t entryB = *(size_t*)b;
    long long dateA = date_to_number(g_journal.entries[entryA].date);
    long long dateB = date_to_number(g_journal.entries[entryB].date);
    if( dateA != dateB ) return (int) (dateA - dateB);
    return entryA - entryB;
}

int main() {
    errno_t err = 0;
    FILE* fp = NULL;
    cmark_node* doc, * cur;
    size_t i;

    g_journal.entries_capacity = 1000;
    if (!(g_journal.entries = calloc(g_journal.entries_capacity, sizeof(journal_entry)))) { perror("calloc"); return ENOMEM; }
    if ((err = fopen_s(&fp, "journal.md", "rb")) && !fp) { perror("fopen"); return err; }

    doc = cmark_parse_file(fp, 0);
    fclose(fp);

    cur = cmark_node_first_child(doc);
    while (cur) {
        cmark_node* next = cmark_node_next(cur);

        if (cmark_node_get_type(cur) == CMARK_NODE_HEADING && cmark_node_get_heading_level(cur) == 1) {

            if( ++g_journal.entries_count > g_journal.entries_capacity ) {
                journal_entry* old_entries = g_journal.entries;
                size_t old_capacity = g_journal.entries_capacity;
                g_journal.entries_capacity *= 2;
                g_journal.entries = (journal_entry*)realloc(g_journal.entries, sizeof(journal_entry) * g_journal.entries_capacity);
                if (g_journal.entries == NULL) { perror("realloc"); return ENOMEM; }
                memset(g_journal.entries + old_capacity, 0, sizeof(journal_entry) * (g_journal.entries_capacity - old_capacity));
            }
            g_journal.entries[g_journal.entries_count-1].document = cmark_node_new(CMARK_NODE_DOCUMENT);
            assert(g_journal.entries[g_journal.entries_count-1].document != NULL);

            cmark_node* txt = cmark_node_first_child(cur);
            g_journal.entries[g_journal.entries_count-1].title = cmark_node_get_literal(txt);
            g_journal.entries[g_journal.entries_count-1].slug = slugify_title(g_journal.entries[g_journal.entries_count-1].title);

            if ((cur = cmark_node_next(cur)) && cmark_node_get_type(cur) == CMARK_NODE_PARAGRAPH) {

                txt = cmark_node_first_child(cur);
                while( txt ) {
                    if (cmark_node_get_type(txt) == CMARK_NODE_TEXT && strchr(cmark_node_get_literal(txt), ':')) {
                        parse_kv_lines(cmark_node_get_literal(txt), &g_journal.entries[g_journal.entries_count-1]);
                    }
                    txt = cmark_node_next(txt);
                }

                next = cmark_node_next(cur);
                cmark_node_unlink(cur);
                cmark_node_free(cur);
            }
        } else if (g_journal.entries_count) {
            cmark_node_unlink(cur);
            cmark_node_append_child(g_journal.entries[g_journal.entries_count-1].document, cur);
        }

        cur = next;
    }

    for (i = 0; i < g_journal.entries_count; ++i) {
        cmark_iter* it = cmark_iter_new(g_journal.entries[i].document);
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
                size_t len = strlen(link), new_len = len + 5; /* "img/" + link */
                char* new_link = calloc(1, new_len);
                if( new_link ) {
                    strncpy("img/", new_link, len);
                    strncat(new_link, link, len);
                    cmark_node_set_url(nod, new_link);
                }
            }
        }
        cmark_iter_free(it);
    }

    for (i = 0; i < g_journal.entries_count; ++i) {
        char* html = cmark_render_html(g_journal.entries[i].document, CMARK_OPT_DEFAULT);
        g_journal.entries[i].content = html;
    }

    g_journal.entries_by_date = calloc(g_journal.entries_count, sizeof(*g_journal.entries_by_date));
    for (i = 0; i < g_journal.entries_count; ++i) {
        g_journal.entries_by_date[i] = i;
    }
    qsort(g_journal.entries_by_date, g_journal.entries_count, sizeof(*g_journal.entries_by_date), compare_entries_by_date);
}

