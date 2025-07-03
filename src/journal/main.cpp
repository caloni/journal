#include "journal.h"
#include <cmark.h>
#include <parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <string>
#include <vector>

#define MAX_LINE_LEN 4096

int main(int argc, char *argv[])
{
    FILE* fp = fopen("journal.md", "r");
    if (!fp)
    {
        perror("fopen");
        return EXIT_FAILURE;
    }

    std::map<std::string, cmark_node*> postsContentBySlug;

    char buf[MAX_LINE_LEN];
    cmark_parser* docParser = cmark_parser_new(CMARK_OPT_DEFAULT);
    cmark_parser* postParser = nullptr;
    Journal journal;
    JournalEntry entry;
    while (fgets(buf, sizeof buf, fp))
    {
        size_t len = strlen(buf);
        cmark_parser_feed(docParser, buf, len);
        if (docParser->current->type == CMARK_NODE_HEADING && docParser->current->as.heading.level == 1)
        {
            if( postParser )
            {
                postsContentBySlug[entry.get_slug()] = cmark_parser_finish(postParser);
                cmark_parser_free(postParser);
                journal.add_entry(entry);
                entry = JournalEntry();
            }
            postParser = cmark_parser_new(CMARK_OPT_DEFAULT);
            entry.set_title_and_slug(buf);
        }
        else
        {
            cmark_parser_feed(postParser, buf, len);
        }
    }
    if (postParser)
    {
        postsContentBySlug[entry.get_slug()] = cmark_parser_finish(postParser);
        cmark_parser_free(postParser);
        journal.add_entry(entry);
        entry = JournalEntry();
    }

    if (ferror(fp))
    {
        perror("fgets or docParser");
        fclose(fp);
        return EXIT_FAILURE;
    }

    cmark_node* document = cmark_parser_finish(docParser);
    for (auto& entry: journal.GetEntries() )
    {
        char* html = cmark_render_html(postsContentBySlug[entry.get_slug()], CMARK_OPT_DEFAULT);
        fputs(entry.get_title().c_str(), stdout);
        fputs("-----------------------------------------", stdout);
        fputs(html, stdout);
        fputs("-----------------------------------------", stdout);
        //getchar();
    }

    cmark_parser_free(docParser);
    fclose(fp);
    return EXIT_SUCCESS;
}

