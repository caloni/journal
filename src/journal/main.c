#include <cmark.h>
#include <tinytemplate.h>
#include <parser.h>
#include <ctype.h>

#define COUNT(X) ((int) (sizeof(X) / sizeof((X)[0])))

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

typedef struct journal_output {
    long* months;
    size_t months_count;
} journal_output;


static journal g_journal;
static journal_output g_journal_output;
const char* MONTHS_TEMPLATE_PATH = "blog\\_months.html";
const char* MONTHS_TEMPLATE_FINAL = "public\\blog\\months.html";

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

long date_to_month_number(const char* date) {
    char buf[7];
    if( date == NULL ) return UINT_MAX;
    buf[0] = date[0], buf[1] = date[1], buf[2] = date[2], buf[3] = date[3]; /* YYYY */
    buf[4] = date[5], buf[5] = date[6]; /* MM */
    buf[6] = '\0';
    return strtol(buf, NULL, 10);
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
    return (int)(entryA - entryB);
}

char* read_file_to_string(const char* path, long* len) {
    FILE* f;
    char* buf;

    f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    *len = ftell(f);
    rewind(f);

    *len = min(*len, ULONG_MAX-1);
    buf = malloc(*len+1);
    if (!buf) return fclose(f), NULL;

    fread(buf, 1, *len, f);
    buf[*len] = '\0';
    fclose(f);
    return buf;
}

void write_file_from_string(const char* path, const char* content) {
    FILE* f;
    long len;

    f = fopen(path, "wb");
    if (!f) return;

    len = (long) strlen(content);
    fwrite(content, len, 1, f);
    fclose(f);
}

void compile_tinytemplate_program() {
    char message[128];
    tinytemplate_instr_t prog[32];
    tinytemplate_status_t status;

    static const char text[] = "Hello, my name is {{name}}!";

    size_t num_instr = 0;
    status = tinytemplate_compile(text, strlen(text), prog, COUNT(prog),
                                  NULL, message, sizeof(message));
    if (status != TINYTEMPLATE_STATUS_DONE) {
        fprintf(stderr, "Error: %s", message);
        return;
    }

    fprintf(stdout, "Program compiled to %ld instructions!\n", (long) num_instr);
    return;
}

int write_months_page() {
    char* document;
    long document_length;

    if (!(document = read_file_to_string(MONTHS_TEMPLATE_PATH, &document_length))) { perror("fopen blog/_months.html"); return EIO; }
    compile_tinytemplate_program();
    /*
    begin = strstr(document, MONTHS_TEMPLATE_BEGIN), end = strstr(document, MONTHS_TEMPLATE_END);
    if (!begin || !end || begin >= end) { perror("write_months_page: invalid template file"); return EINVAL; }
    document_length_final = g_journal_output.months_count * 100 + document_length;
    if (!(final = malloc(document_length_final + 1))) { perror("write_months_page: malloc final"); return ENOMEM; }
    strncpy(final, document, begin - document);
    final[begin - document] = '\0';
    for(i = g_journal_output.months_count - 1, year = 0; i >= 0; --i) {
        int cur_year = g_journal_output.months[i] / 100;
        int cur_month = g_journal_output.months[i] % 100;
        char month_str[200];
        if( cur_year != year ) {
            if( year != 0 ) { strcat(final, "\n</p>"); }
            snprintf(month_str, sizeof(month_str), "\n<p id=\"%04d-%02d\" class=\"toc\"><strong>%04d</strong>", cur_year, cur_month, cur_year);
            strcat(final, month_str);
            year = cur_year;
        }
        snprintf(month_str, sizeof(month_str), "\n<a href=\"%04d-%02d.html\"> %02d </a>", cur_year, cur_month, cur_month);
        strcat(final, month_str);
    }
    strcat(final, "\n</p>\n");
    strcat(final, end + strlen(MONTHS_TEMPLATE_END));
    write_file_from_string(MONTHS_TEMPLATE_FINAL, final);
    */
    return 0;
}

int main() {
    int err;
    char* document;
    long document_length;
    cmark_node* cmark_node_doc, * cmark_node_cur;
    size_t i, j;

    g_journal.entries_capacity = 1000;
    if (!(g_journal.entries = calloc(g_journal.entries_capacity, sizeof(journal_entry)))) { perror("calloc"); return ENOMEM; }
    if (!(document = read_file_to_string("journal.md", &document_length))) { perror("fopen journal.md"); return EIO; }
    if (!(cmark_node_doc = cmark_parse_document(document, document_length, 0))) { perror("cmark_parse_document"); return EINVAL; }

    cmark_node_cur = cmark_node_first_child(cmark_node_doc);
    while (cmark_node_cur) {
        cmark_node* next = cmark_node_next(cmark_node_cur);

        if (cmark_node_get_type(cmark_node_cur) == CMARK_NODE_HEADING && cmark_node_get_heading_level(cmark_node_cur) == 1) {

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

            cmark_node* txt = cmark_node_first_child(cmark_node_cur);
            g_journal.entries[g_journal.entries_count-1].title = cmark_node_get_literal(txt);
            g_journal.entries[g_journal.entries_count-1].slug = slugify_title(g_journal.entries[g_journal.entries_count-1].title);

            if ((cmark_node_cur = cmark_node_next(cmark_node_cur)) && cmark_node_get_type(cmark_node_cur) == CMARK_NODE_PARAGRAPH) {

                txt = cmark_node_first_child(cmark_node_cur);
                while( txt ) {
                    if (cmark_node_get_type(txt) == CMARK_NODE_TEXT && strchr(cmark_node_get_literal(txt), ':')) {
                        parse_kv_lines(cmark_node_get_literal(txt), &g_journal.entries[g_journal.entries_count-1]);
                    }
                    txt = cmark_node_next(txt);
                }

                next = cmark_node_next(cmark_node_cur);
                cmark_node_unlink(cmark_node_cur);
                cmark_node_free(cmark_node_cur);
            }
        } else if (g_journal.entries_count) {
            cmark_node_unlink(cmark_node_cur);
            cmark_node_append_child(g_journal.entries[g_journal.entries_count-1].document, cmark_node_cur);
        }

        cmark_node_cur = next;
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

    if( g_journal.entries_count == 0 ) {
        fprintf(stderr, "No journal entries found.\n");
        return 1;
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

    g_journal_output.months = calloc(g_journal.entries_count, sizeof(*g_journal_output.months));
    g_journal_output.months[0] = date_to_month_number(g_journal.entries[g_journal.entries_by_date[0]].date);
    for (i = 0, j = 0; i < g_journal.entries_count; ++i) {
        long month = date_to_month_number(g_journal.entries[g_journal.entries_by_date[i]].date);
        if( month != g_journal_output.months[j] && j+1 < g_journal.entries_count ) {
            g_journal_output.months[++j] = month;
        }
    }
    g_journal_output.months_count = j + 1;

    if ((err = write_months_page()) != 0) { perror("write_months_page"); return err; }
}

