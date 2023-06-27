// awkblog
// how to use: gawk -f scripts\adoc2html.awk -l awkblog content\blog.txt
#include "constants.h"
#include <string>
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <map>
#include <filesystem>
#include <awklib/awk.h>
#include <basetsd.h>
#include <Windows.h>

typedef SSIZE_T ssize_t;
#include "gawkapi.h"


using namespace std;
using namespace chrono;
namespace fs = std::filesystem;


BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved)  // reserved
{
    if( fdwReason == DLL_PROCESS_ATTACH )
    {
        int x = 0;
    }
    return TRUE;
}

/* Boilerplate code: */
int plugin_is_GPL_compatible;

static gawk_api_t const * api;
static awk_ext_id_t ext_id;
static const char *ext_version = NULL; /* or ... = "some string" */

awk_value_t* do_name(int num_actual_args, awk_value_t* result, struct awk_ext_func* finfo)
{
    if( num_actual_args > 0 )
    {
        awk_valtype_t wanted = AWK_UNDEFINED;
        awk_value_t value = {};
        awk_bool_t res = api->api_get_argument(ext_id, 0, wanted, &value);
        if( res == awk_true )
        {
            if( value.val_type == AWK_ARRAY )
            {
                size_t count = 0;
                res = api->api_get_element_count(ext_id, &value, &count);

                if( res == awk_true )
                {
                    for (size_t i = 0; i < count; ++i)
                    {
                        awk_value_t index = { AWK_NUMBER };
                        index.num_value = count;
                        awk_value_t element = {};
                        res = api->api_get_array_element(ext_id, &value, &index, wanted, &element);
                    }
                }
            }
        }
    }
    return NULL;
}

static awk_ext_func_t func_table[] = {
	{ "name", do_name, 1 },
	/* ... */
};

static awk_bool_t init_func()
{
    return awk_true;
}

dl_load_func(func_table, awkblog, "ablog")

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
    auto start = high_resolution_clock::now();
    int postCount = 0;
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
            res = awk_exec(g_interp);

            postCount = stoi(GetVar("idx"));
            for (int i = 1; i <= postCount; ++i)
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
            }

            awk_end(g_interp);
        }
        else
        {
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

                string ppath = "public/blog_alpha/blog_entry_" + to_string(++postCount) + ".html";
                FlushContent(ppath, rawPost);
            }
        }
    }

    auto end = high_resolution_clock::now();
    cout << postCount << " posts published in " << duration_cast<seconds>(end - start).count() << " seconds\n";
}

