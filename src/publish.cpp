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

string GetVar(const string& name, const string& index = "")
{
    string value;
    awksymb symb{};
    symb.name = name.c_str();
    if( index.size() )
        symb.index = index.c_str();
    int res = awk_getvar(g_interp, &symb);
    if (res == 1 )
    {
        if( symb.sval )
        {
            value = symb.sval;
        }
        else if ( symb.fval )
        {
            value = to_string(symb.fval);
        }
    }
    return value;
}


std::vector<std::string> GetPostLines(const std::string& str)
{
    auto result = std::vector<std::string>{};
    auto ss = std::stringstream{ str };

    for (std::string line; std::getline(ss, line, '\n');)
        result.push_back(line);

    return result;
}

void FlushContent(const string& ppath, const string& content)
{
    ifstream pifs(ppath);
    if (pifs)
    {
        string postContent = ReadEntireFile(pifs);

        if (content != postContent)
        {
            ofstream ofs(ppath);
            if (ofs)
            {
                ofs << content;
            }
        }
    }
    else
    {
        ofstream ofs(ppath);
        if (ofs)
        {
            ofs << content;
        }
    }
}

int main()
{
    fs::create_directories("public/blog_alpha");
    ifstream ifs("content/blog.txt");

    if (ifs)
    {
        string content = ReadEntireFile(ifs);
        bool useAwk = true;

        if( useAwk )
        {
            g_interp = awk_init(NULL);
            g_is = istringstream(content);
            awk_infunc(g_interp, myinproc);

            string program = R"(
/^= / { title[++idx] = substr($0, 3) }
/^[^=]+/ { content[idx] = content[idx] "<p>" $0 "</p>\n" }
)";

            int res = awk_setprog(g_interp, program.c_str());
            res = awk_compile(g_interp);
            cout << "parsing...";
            res = awk_exec(g_interp);
            cout << " done\n";

            int posts = stoi(GetVar("idx"));
            for (int i = 1; i <= posts; ++i)
            {
                string idx = to_string(i);
                string title = GetVar("title", idx);
                string content = GetVar("content", idx);

                ostringstream os;
                os << HTML_HEAD << '\n'
                    << HTML_BODY_TOP << '\n'
                    << content << '\n'
                    << HTML_BODY_BOTTOM << endl;
                content = os.str();

                string ppath = "public/blog_alpha/blog_entry_" + idx + ".html";
                FlushContent(ppath, content);
                cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << idx;
            }

            awk_end(g_interp);
        }
        else
        {
            int counter = 0;
            while (content.size())
            {
                ostringstream os;
                string rawPost = GetNextPost(content);

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

                string ppath = "public/blog_alpha/blog_entry_" + to_string(++counter) + ".html";
                FlushContent(ppath, rawPost);
                cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << counter;
            }
        }
    }
}

