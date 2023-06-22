#include "constants.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <filesystem>
#include <awklib/awk.h>

using namespace std;
namespace fs = std::filesystem;

struct Post
{
    map<string, string> header;
    string content;
};

std::string ReadEntireFile(std::ifstream& in)
{
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

string GetNextPost(string& content)
{
    size_t postBegin = content.find("\n= ");
    size_t nextPost = content.find("\n= ", postBegin + 1);
    bool valid = postBegin != content.npos
        && nextPost != content.npos;

    if( valid )
    {
        string post = content.substr(postBegin, nextPost);
        content.erase(0, nextPost);
        return post;
    }
    else
    {
        string post = content;
        content.clear();
        return post;
    }
}


AWKINTERP* g_interp;
istringstream g_is;
static int myinproc()
{
    return g_is.get();
}

string GetVar(const string& name)
{
    string value;
    awksymb symb{};
    symb.name = name.c_str();
    int res = awk_getvar(g_interp, &symb);
    if (res == 1 && symb.sval)
    {
        value = symb.sval;
    }
    return value;
}

Post ParsePost(const std::string& rawPost)
{
    g_interp = awk_init(NULL);
    g_is = istringstream(rawPost);
    awk_infunc(g_interp, myinproc);

    string program = R"(
/^= / { title = substr($0, 3) }
/^[^=]+/ { content = content "<p>" $0 "</p>\n" }
)";

    int res = awk_setprog(g_interp, program.c_str());
    res = awk_compile(g_interp);
    res = awk_exec(g_interp);
    Post post;
    post.header["title"] = GetVar("title");
    post.content = GetVar("content");
    awk_end(g_interp);

    return post;
}


std::vector<std::string> GetPostLines(const std::string& str)
{
    auto result = std::vector<std::string>{};
    auto ss = std::stringstream{ str };

    for (std::string line; std::getline(ss, line, '\n');)
        result.push_back(line);

    return result;
}

int main()
{
    fs::create_directories("public/blog_alpha");
    ifstream ifs("content/blog.txt");

    if (ifs)
    {
        string content = ReadEntireFile(ifs);
        int counter = 0;
        while (content.size())
        {
            ostringstream os;
            string rawPost = GetNextPost(content);
            bool useAwk = false;

            if( useAwk )
            {
                Post post = ParsePost(rawPost);
                os << HTML_HEAD << '\n'
                    << HTML_BODY_TOP << '\n'
                    << post.content << '\n'
                    << HTML_BODY_BOTTOM << endl;
                rawPost = os.str();
            }
            else
            {
                vector<string> postLines = GetPostLines(rawPost);
                os << HTML_HEAD << endl
                    << HTML_BODY_TOP << endl;
                for (string& line : postLines)
                {
                    if (line.size() > 2 && line[0] != '=' && line[1] != ' ')
                    {
                        os << "<p>" << line << "</p>" << endl;
                    }
                }
                os << HTML_BODY_BOTTOM << endl;
                rawPost = os.str();
            }

            string ppath = "public/blog_alpha/blog_entry_" + to_string(++counter) + ".html";
            cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << counter;
            ifstream pifs(ppath);
            if (pifs)
            {
                string postContent = ReadEntireFile(pifs);

                if( rawPost != postContent )
                {
                    ofstream ofs(ppath);
                    if (ofs)
                    {
                        ofs << rawPost;
                    }
                }
            }
            else
            {
                ofstream ofs(ppath);
                if (ofs)
                {
                    ofs << rawPost;
                }
            }
        }
    }
}

